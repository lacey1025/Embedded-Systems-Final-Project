package com.example.a5143finalproject.ui

import androidx.lifecycle.ViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.update
import java.time.LocalTime
import android.util.Log
import androidx.lifecycle.viewModelScope
import com.example.a5143finalproject.data.EspUpdateSettings
import com.example.a5143finalproject.data.RetrofitClient
import kotlinx.coroutines.launch
import kotlinx.coroutines.withTimeoutOrNull
import java.time.format.DateTimeFormatter
import java.time.format.DateTimeParseException

class BiomeViewModel : ViewModel() {
    private val _biomeUiState = MutableStateFlow(BiomeUiState()) // singleton variable of UiState
    val biomeUiState: StateFlow<BiomeUiState> = _biomeUiState.asStateFlow() // editable copy of UiState

    // These methods update the singleton to ensure only the View Model updates the current UiState
    fun updateLight(light: Int) {
        _biomeUiState.update { it.copy(light = light) }
    }

    fun updateMaxHumidity(humidity: Int) {
        _biomeUiState.update { it.copy(maxHumidity = humidity) }
    }

    fun updateMinHumidity(humidity: Int) {
        _biomeUiState.update { it. copy(minHumidity = humidity) }
    }

    fun updateMaxTemperature(temperature: Int) {
        _biomeUiState.update { it.copy(maxTemperature = temperature) }
    }

    fun updateMinTemperature(temperature: Int) {
        _biomeUiState.update { it.copy(minTemperature = temperature) }
    }

    fun updateLightOnTime(time: LocalTime) {
        _biomeUiState.update { it.copy(lightOnTime = time) }
    }

    fun updateLightOffTime(time: LocalTime) {
        _biomeUiState.update { it.copy(lightOffTime = time) }
    }

    // Makes a connection, then gets data from the ESP32
    fun updateDataFromESP32() {
        viewModelScope.launch {
            _biomeUiState.update { it.copy(isLoading = true, errorMessage = null) }

            try {
                // Use timeout to attempt connection
                val result = withTimeoutOrNull(5000) {
                    try {
                        val response = RetrofitClient.instance.getData()
                        _biomeUiState.update {
                            it.copy(
                                currentLight = response.currentLight,
                                currentHumidity = response.currentHumidity,
                                currentTemperature = response.currentTemperature,
                                lightOnTime = response.lightOnTime.parseToLocalTime(),
                                lightOffTime = response.lightOffTime.parseToLocalTime(),
                                light = response.light,
                                maxHumidity = response.maxHumidity,
                                minHumidity = response.minHumidity,
                                maxTemperature = response.maxTemperature,
                                minTemperature = response.minTemperature,
                                isLoading = false,
                                firstTime = false,
                                errorMessage = null,
                                isTimeout = false
                            )
                        }
                        true // Indicate success
                    } catch (e: Exception) {
                        Log.e("BiomeViewModel", "Exception during data fetch: ${e.message}", e)
                        _biomeUiState.update { it.copy(isLoading = false, errorMessage = "Could not connect to ESP32") }
                        null // Indicate failure
                    }
                }

                // Timeout reached, show error if result is null
                if (result == null) {
                    Log.e("BiomeViewModel", "Connection timed out")
                    _biomeUiState.update { it.copy(isLoading = false, errorMessage = "Connection timed out", isTimeout = true ) }
                }
            } catch (e: Exception) {
                Log.e("BiomeViewModel", "Exception during data fetch: ${e.message}", e)
                _biomeUiState.update { it.copy(isLoading = false, errorMessage = "An unexpected error occurred") }
            }
        }
    }


    // Submits user selections to the ESP32
    fun submitChanges() {
        viewModelScope.launch {
            try {
                val settings = EspUpdateSettings(
                    lightOnTime = biomeUiState.value.lightOnTime.localTimeToString(),
                    lightOffTime = biomeUiState.value.lightOffTime.localTimeToString(),
                    light = biomeUiState.value.light,
                    maxTemperature = biomeUiState.value.maxTemperature,
                    minTemperature = biomeUiState.value.minTemperature,
                    maxHumidity = biomeUiState.value.maxHumidity,
                    minHumidity = biomeUiState.value.minHumidity
                )

                val response = RetrofitClient.instance.updateSettings(settings)
                if (response.isSuccessful) {
                    Log.d("BiomeViewModel", "Settings updated successfully")
                    _biomeUiState.update { it.copy(isSuccess = true) }
                } else {
                    Log.e("BiomeViewModel", "Failed to update settings: ${response.code()}")
                    _biomeUiState.update { it.copy(isFail = true) }
                }
            } catch (e: Exception) {
                Log.e("BiomeViewModel", "Error updating settings", e)
                _biomeUiState.update { it.copy(isFail = true) }
            }
        }
    }

    fun resetSuccessFlag() {
        _biomeUiState.update { it.copy(isSuccess = false) }
    }
    fun resetFailFlag() {
        _biomeUiState.update { it.copy(isFail = false) }
    }
}


// This data class is used to store the UI state of the app. It contains the threshold data,
// as well as flags to ensure correct behavior within the application
data class BiomeUiState(
    val currentLight: Int = 0,
    val currentHumidity: Int = 0,
    val currentTemperature: Int = 0,
    val lightOnTime: LocalTime = LocalTime.of(8, 0),
    val lightOffTime: LocalTime = LocalTime.of(20, 0),
    val light: Int = 10764,
    val maxHumidity: Int = 0,
    val minHumidity: Int = 0,
    val maxTemperature: Int = 0,
    val minTemperature: Int = 0,
    val isLoading: Boolean = false,
    val isSuccess: Boolean = false,
    val isFail: Boolean = false,
    val errorMessage: String? = null,
    val isTimeout: Boolean = false,
    val firstTime: Boolean = true
)

// Converters
fun LocalTime.localTimeToString(): String {
    return this.format(DateTimeFormatter.ofPattern("HHmm"))
}

fun String.parseToLocalTime(): LocalTime {
    return try {
        LocalTime.parse(this, DateTimeFormatter.ofPattern("HHmm"))
    } catch (e: DateTimeParseException) {
        LocalTime.of(8, 0)
    }
}