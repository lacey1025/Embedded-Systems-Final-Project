package com.example.a5143finalproject.ui

import android.app.TimePickerDialog
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.BrightnessHigh
import androidx.compose.material.icons.filled.Thermostat
import androidx.compose.material.icons.filled.WaterDrop
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.ExposedDropdownMenuBox
import androidx.compose.material3.ExposedDropdownMenuDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Slider
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import kotlinx.coroutines.launch
import java.time.LocalTime

// This screen allows the user to change light, max and min humidity and termperature,
// as well as times to turn on and off the system
@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun BiomeThresholdScreen(
    uiState: BiomeUiState,
    onUpdateLight: (Int) -> Unit,
    onUpdateMaxHumidity: (Int) -> Unit,
    onUpdateMinHumidity: (Int) -> Unit,
    onUpdateMaxTemperature: (Int) -> Unit,
    onUpdateMinTemperature: (Int) -> Unit,
    onSubmit: () -> Unit,
    onUpdateLightOnTime: (LocalTime) -> Unit,
    onUpdateLightOffTime: (LocalTime) -> Unit,
    modifier: Modifier = Modifier
) {
    var editLight by remember { mutableStateOf(uiState.light) }
    var editMaxHumidity by remember { mutableStateOf(uiState.maxHumidity) }
    var editMinHumidity by remember { mutableStateOf(uiState.minHumidity) }
    var editMaxTemperature by remember { mutableStateOf(uiState.maxTemperature) }
    var editMinTemperature by remember { mutableStateOf(uiState.minTemperature) }
    var editLightOnTime by remember { mutableStateOf(uiState.lightOnTime) }
    var editLightOffTime by remember { mutableStateOf(uiState.lightOffTime) }

    // Presets that autofill the scroll bars when selected
    val presets = mapOf(
        "Vegetative Phase" to Preset(1600, 70, 50, 90, 82),
        "Flowering Phase" to Preset(1600, 60, 40, 72, 55)
    )
    var selectedPreset by remember { mutableStateOf("None") }
    var expanded by remember { mutableStateOf(false) }

    val scrollState = rememberSaveable { mutableStateOf(0) }
    val coroutineScope = rememberCoroutineScope()

    LaunchedEffect(Unit) {
        coroutineScope.launch {
            scrollState.value = 0
        }
    }

    Column(
        modifier = modifier
            .fillMaxSize()
            .verticalScroll(rememberScrollState(scrollState.value))
            .padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {

        ExposedDropdownMenuBox(
            expanded = expanded,
            onExpandedChange = { expanded = !expanded }
        ) {
            OutlinedTextField(
                value = selectedPreset,
                onValueChange = {},
                readOnly = true,
                label = { Text("Preset") },
                trailingIcon = { ExposedDropdownMenuDefaults.TrailingIcon(expanded = expanded) },
                modifier = Modifier
                    .menuAnchor()
                    .fillMaxWidth()
            )
            ExposedDropdownMenu(
                expanded = expanded,
                onDismissRequest = { expanded = false }
            ) {
                presets.keys.forEach { presetName ->
                    DropdownMenuItem(
                        onClick = {
                            selectedPreset = presetName
                            expanded = false
                            presets[presetName]?.let { preset ->
                                editLight = preset.light
                                onUpdateLight(editLight)
                                editMaxHumidity = preset.maxHumidity
                                onUpdateMaxHumidity(editMaxHumidity)
                                editMinHumidity = preset.minHumidity
                                onUpdateMinHumidity(editMinHumidity)
                                editMaxTemperature = preset.maxTemperature
                                onUpdateMaxTemperature(editMaxTemperature)
                                editMinTemperature = preset.minTemperature
                                onUpdateMinTemperature(editMinTemperature)
                            }
                        },
                        text = { Text(presetName) }
                    )
                }
            }
        }

        // Light Section
        ThresholdCard(
            title = "Light",
            icon = Icons.Default.BrightnessHigh,
            valueText = "$editLight lumens",
            value = editLight,
            range = 0..1600,
            onValueChange = {
                editLight = it
                onUpdateLight(editLight)
            }
        )


        // Humidity Section
        ThresholdCard(
            title = "Max Humidity",
            icon = Icons.Default.WaterDrop,
            valueText = "$editMaxHumidity%",
            value = editMaxHumidity,
            range = 0..100,
            onValueChange = {
                editMaxHumidity = it
                onUpdateMaxHumidity(editMaxHumidity)

                if (editMinHumidity > editMaxHumidity) {
                    editMinHumidity = editMaxHumidity
                    onUpdateMinHumidity(editMinHumidity)
                }
            }
        )

        ThresholdCard(
            title = "Min Humidity",
            icon = Icons.Default.WaterDrop,
            valueText = "$editMinHumidity%",
            value = editMinHumidity,
            range = 0..100,
            onValueChange = {
                editMinHumidity = it
                onUpdateMinHumidity(editMinHumidity)

                if (editMaxHumidity < editMinHumidity) {
                    editMaxHumidity = editMinHumidity
                    onUpdateMaxHumidity(editMaxHumidity)
                }
            }
        )

        // Temperature Section
        ThresholdCard(
            title = "Max Temperature",
            icon = Icons.Default.Thermostat,
            valueText = "$editMaxTemperature°F",
            value = editMaxTemperature,
            range = 50..100,
            onValueChange = {
                editMaxTemperature = it
                onUpdateMaxTemperature(editMaxTemperature)

                if (editMinTemperature > editMaxTemperature) {
                    editMinTemperature = editMaxTemperature
                    onUpdateMinTemperature(editMinTemperature)
                }
            }
        )

        ThresholdCard(
            title = "Min Temperature",
            icon = Icons.Default.Thermostat,
            valueText = "$editMinTemperature°F",
            value = editMinTemperature,
            range = 50..100,
            onValueChange = {
                editMinTemperature = it
                onUpdateMinTemperature(editMinTemperature)

                if (editMaxTemperature < editMinTemperature) {
                    editMaxTemperature = editMinTemperature
                    onUpdateMaxTemperature(editMaxTemperature)
                }
            }
        )

        // Light On Time Section
        TimePickerCard(
            title = "Light On Time",
            time = editLightOnTime,
            onTimeSelected = { newTime ->
                editLightOnTime = newTime
                onUpdateLightOnTime(editLightOnTime)
            }
        )

        // Light Off Time Section
        TimePickerCard(
            title = "Light Off Time",
            time = editLightOffTime,
            onTimeSelected = { newTime ->
                editLightOffTime = newTime
                onUpdateLightOffTime(editLightOffTime)
            }
        )
        Button(
            onClick = onSubmit,
            colors = ButtonDefaults.buttonColors(containerColor = MaterialTheme.colorScheme.tertiary),
            modifier = Modifier
                .padding(top = 20.dp)
                .fillMaxWidth()
        ) {
            Text("Submit Changes")
        }
    }
}

@Composable
fun ThresholdCard(
    title: String,
    icon: ImageVector,
    valueText: String,
    value: Int,
    range: IntRange,
    onValueChange: (Int) -> Unit,
    modifier: Modifier = Modifier
) {
    Card(
        elevation = CardDefaults.cardElevation(4.dp),
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
        modifier = modifier.fillMaxWidth()
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            Row(verticalAlignment = Alignment.CenterVertically) {
                Icon(icon, contentDescription = title, modifier = Modifier.size(24.dp))
                Text(
                    text = title,
                    fontSize = 24.sp,
                    modifier = Modifier.padding(start = 8.dp)
                )
            }
            Slider(
                value = value.toFloat(),
                onValueChange = { onValueChange(it.toInt()) },
                valueRange = range.first.toFloat()..range.last.toFloat(),
                modifier = Modifier.fillMaxWidth()
            )
            Text(
                text = valueText,
                fontSize = 16.sp,
                textAlign = TextAlign.Center,
                modifier = Modifier.fillMaxWidth()
            )
        }
    }
}

@Composable
fun TimePickerCard(
    title: String,
    time: LocalTime,
    onTimeSelected: (LocalTime) -> Unit,
    modifier: Modifier = Modifier
) {
    Card(
        elevation = CardDefaults.cardElevation(4.dp),
        colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surfaceVariant),
        modifier = modifier.fillMaxWidth()
    ) {
        Column(
            modifier = Modifier.padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            Text(
                text = title,
                fontSize = 24.sp,
                modifier = Modifier.padding(bottom = 8.dp)
            )
            val context = LocalContext.current
            Button(
                onClick = {
                    val timePickerDialog = TimePickerDialog(context, { _, hour, minute ->
                        onTimeSelected(LocalTime.of(hour, minute))
                    }, time.hour, time.minute, true)
                    timePickerDialog.show()
                },
                modifier = Modifier.fillMaxWidth()
            ) {
                Text(text = "Set Time: ${time}")
            }
        }
    }
}

// Data class for preset settings
data class Preset(
    val light: Int,
    val maxHumidity: Int,
    val minHumidity: Int,
    val maxTemperature: Int,
    val minTemperature: Int
)