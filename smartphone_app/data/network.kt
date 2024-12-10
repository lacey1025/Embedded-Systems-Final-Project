package com.example.a5143finalproject.data
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import retrofit2.Response
import retrofit2.http.Body
import retrofit2.http.GET
import retrofit2.http.POST

// When a get request is made, run getData method, when POST request is made, run updateSettings method
interface ESP32Service {
    @GET("/data")
    suspend fun getData(): EspResponse

    @POST("/update")
    suspend fun updateSettings(@Body settings: EspUpdateSettings): Response<Void>
}

object RetrofitClient {
    private const val BASE_URL = "http://192.168.0.45/" // base url of ESP32

    private val retrofit: Retrofit by lazy {
        Retrofit.Builder()
            .baseUrl(BASE_URL)
            .addConverterFactory(GsonConverterFactory.create())
            .build()
    }

    val instance: ESP32Service by lazy {
        retrofit.create(ESP32Service::class.java)
    }
}

// Stores the data collected from the ESP32 GET request
data class EspResponse (
    val currentLight: Int = 0,
    val currentHumidity: Int = 0,
    val currentTemperature: Int = 0,
    val lightOnTime: String = "",
    val lightOffTime: String = "",
    val light: Int = 0,
    val maxHumidity: Int = 0,
    val minHumidity: Int = 0,
    val maxTemperature: Int = 0,
    val minTemperature: Int = 0
)

// Stores data needed to send POST request
data class EspUpdateSettings (
    val lightOnTime: String = "",
    val lightOffTime: String = "",
    val light: Int = 0,
    val maxHumidity: Int = 0,
    val minHumidity: Int = 0,
    val maxTemperature: Int = 0,
    val minTemperature: Int = 0
)