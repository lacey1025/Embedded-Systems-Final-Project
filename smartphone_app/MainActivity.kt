package com.example.a5143finalproject

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Surface
import androidx.compose.ui.Modifier
import com.example.a5143finalproject.ui.BiomeApp
import com.example.a5143finalproject.ui.theme._5143FinalProjectTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        enableEdgeToEdge()
        super.onCreate(savedInstanceState)
        setContent {
            _5143FinalProjectTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                ) {
                    BiomeApp() // starts the biome application -> look at the BiomeApp file under ui directory
                }
            }
        }
    }
}