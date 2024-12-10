package com.example.a5143finalproject.ui

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.compose.LocalLifecycleOwner
import androidx.lifecycle.repeatOnLifecycle
import kotlinx.coroutines.delay


// This screen displays Light, Temperature, and Humidity readings from the ESP32. These readings
// are first collected by the atMega3208 and sent to the app over Wifi
@Composable
fun BiomeCurrentStatusScreen(uiState: BiomeUiState, viewModel: BiomeViewModel, modifier: Modifier = Modifier) {
    val lifecycleOwner = LocalLifecycleOwner.current

    // Updates data every second when this screen is visible
    LaunchedEffect(lifecycleOwner) {
        lifecycleOwner.lifecycle.repeatOnLifecycle(Lifecycle.State.STARTED) {
            while (true) {
                delay(1000L)
                viewModel.updateDataFromESP32()
            }
        }
    }

    Column(
        modifier = modifier.padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(8.dp)
    ) {
        LightCard(currentLight = uiState.currentLight)
        HumidityCard(currentHumidity = uiState.currentHumidity)
        TemperatureCard(currentTemperature = uiState.currentTemperature)
    }
}

@Composable
fun LightCard(currentLight: Int) {
    Card(
        elevation = CardDefaults.cardElevation(defaultElevation = 4.dp),
        modifier = Modifier.fillMaxWidth()
    ) {
        Column(modifier = Modifier.padding(16.dp)) {
            StaticLabelText("Light")
            DynamicText("$currentLight lumens")
        }
    }
}

@Composable
fun HumidityCard(currentHumidity: Int) {
    Card(
        elevation = CardDefaults.cardElevation(defaultElevation = 4.dp),
        modifier = Modifier.fillMaxWidth()
    ) {
        Column(modifier = Modifier.padding(16.dp)) {
            StaticLabelText("Humidity")
            DynamicText("$currentHumidity%")
        }
    }
}

@Composable
fun TemperatureCard(currentTemperature: Int) {
    Card(
        elevation = CardDefaults.cardElevation(defaultElevation = 4.dp),
        modifier = Modifier.fillMaxWidth()
    ) {
        Column(modifier = Modifier.padding(16.dp)) {
            StaticLabelText("Temperature")
            DynamicText("$currentTemperature°F")
        }
    }
}

// Composable to hold static text, which doesn't recompose
@Composable
fun StaticLabelText(text: String) {
    Text(
        text = text,
        fontSize = 36.sp,
        modifier = Modifier.fillMaxWidth(),
        textAlign = TextAlign.Center
    )
}

// Composable to display only the dynamic part of each card
@Composable
fun DynamicText(value: String) {
    Text(
        text = value,
        fontSize = 36.sp,
        modifier = Modifier
            .padding(top = 8.dp)
            .fillMaxWidth(),
        textAlign = TextAlign.Center
    )
}