package com.example.a5143finalproject.ui

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Tab
import androidx.compose.material3.TabRow
import androidx.compose.material3.TabRowDefaults
import androidx.compose.material3.TabRowDefaults.tabIndicatorOffset
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.material3.TopAppBarScrollBehavior
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.input.nestedscroll.nestedScroll
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.NavController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import coil.compose.AsyncImage
import coil.decode.ImageDecoderDecoder
import coil.request.ImageRequest
import com.airbnb.lottie.compose.LottieAnimation
import com.airbnb.lottie.compose.LottieCompositionSpec
import com.airbnb.lottie.compose.animateLottieCompositionAsState
import com.airbnb.lottie.compose.rememberLottieComposition
import com.example.a5143finalproject.R
import kotlinx.coroutines.delay


@Composable
fun BiomeApp(
    viewModel: BiomeViewModel = viewModel()
) {
    val uiState by viewModel.biomeUiState.collectAsState()

    LaunchedEffect(Unit) {
        viewModel.updateDataFromESP32()
    }
    when {
        uiState.firstTime && !uiState.isTimeout -> { LoadingScreen() }
        uiState.isTimeout -> { TimeoutScreen(viewModel = viewModel ) }
        uiState.errorMessage != null -> { ErrorScreen(uiState.errorMessage) }
        else -> {
            BiomeApp(
                uiState = uiState,
                viewModel = viewModel
            )
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
private fun BiomeApp(
    uiState: BiomeUiState,
    modifier: Modifier = Modifier,
    viewModel: BiomeViewModel
) {
    val navController = rememberNavController()
    val scrollBehavior = TopAppBarDefaults.pinnedScrollBehavior()
    var selectedTabIndex by remember { mutableStateOf(0) }

    // Trigger navigation when selectedTabIndex changes
    LaunchedEffect(selectedTabIndex) {
        val screen = when (selectedTabIndex) {
            0 -> BiomeScreens.CURRENT_VALUES.name
            1 -> BiomeScreens.THRESHOLDS.name
            else -> BiomeScreens.CURRENT_VALUES.name
        }
        navController.navigate(screen) {
            popUpTo(navController.graph.startDestinationId) { saveState = true }
            launchSingleTop = true
            restoreState = true
        }
    }

    // Handling Success and Failure
    LaunchedEffect(uiState.isSuccess) {
        if (uiState.isSuccess) {
            navController.navigate("success") {
                popUpTo(navController.graph.startDestinationId) { inclusive = true }
            }
            viewModel.resetSuccessFlag()
        }
        if (uiState.isFail) {
            navController.navigate("failure") {
                viewModel.resetFailFlag()
            }
        }
    }
    // Reset tab index when navigating back to Biome screen
    LaunchedEffect(navController.currentBackStackEntry) {
        if (navController.currentBackStackEntry?.destination?.route == BiomeScreens.CURRENT_VALUES.name) {
            selectedTabIndex = 0
        }
    }

    Scaffold(
        topBar = {
            BiomeTopAppBar(scrollBehavior = scrollBehavior)
        },
        modifier = Modifier
            .nestedScroll(scrollBehavior.nestedScrollConnection)
            .fillMaxSize()
    ) { contentPadding ->
        Column(
            modifier = Modifier
                .padding(contentPadding)
                .fillMaxSize()
        ) {
            TabRow(
                selectedTabIndex = selectedTabIndex,
                modifier = Modifier.fillMaxWidth(),
                indicator = { tabPositions ->
                    TabRowDefaults.Indicator(
                        Modifier.tabIndicatorOffset(tabPositions[selectedTabIndex])
                    )
                }
            ) {
                Tab(
                    text = { Text("Current Status") },
                    selected = selectedTabIndex == 0,
                    onClick = { selectedTabIndex = 0 }
                )
                Tab(
                    text = { Text("Biome Settings") },
                    selected = selectedTabIndex == 1,
                    onClick = { selectedTabIndex = 1 }
                )
            }
            NavHost(
                navController = navController,
                startDestination = BiomeScreens.CURRENT_VALUES.name,
                modifier = Modifier.fillMaxSize()
            ) {
                composable(BiomeScreens.CURRENT_VALUES.name) {
                    BiomeCurrentStatusScreen(uiState = uiState, viewModel = viewModel)
                }
                composable(BiomeScreens.THRESHOLDS.name) {
                    BiomeThresholdScreen(
                        uiState = uiState,
                        onUpdateLight = { viewModel.updateLight(it) },
                        onUpdateMaxHumidity = { viewModel.updateMaxHumidity(it) },
                        onUpdateMinHumidity = { viewModel.updateMinHumidity(it) },
                        onUpdateMaxTemperature = { viewModel.updateMaxTemperature(it) },
                        onUpdateMinTemperature = { viewModel.updateMinTemperature(it) },
                        onUpdateLightOnTime = { viewModel.updateLightOnTime(it) },
                        onUpdateLightOffTime = { viewModel.updateLightOffTime(it) },
                        onSubmit = { viewModel.submitChanges() }
                    )
                }
                composable("success") {
                    SuccessScreen(navController = navController, onResetTabIndex = { selectedTabIndex = 0 })
                }
                composable("failure") {
                    FailureScreen(navController = navController, onResetTabIndex = { selectedTabIndex = 0 })
                }
            }
        }
    }
}

// Entered when threshold settings are successfully updated
@Composable
fun SuccessScreen(
    navController: NavController,
    onResetTabIndex: () -> Unit,
    modifier: Modifier = Modifier
) {
    val composition by rememberLottieComposition(LottieCompositionSpec.RawRes(R.raw.success))
    val progress by animateLottieCompositionAsState(composition)


    LaunchedEffect(Unit) {
        // Add a delay to keep the success screen visible before navigating away
        delay(3000) // Show success for 3 seconds
        onResetTabIndex()
        navController.navigate(BiomeScreens.CURRENT_VALUES.name) {
            popUpTo(BiomeScreens.CURRENT_VALUES.name) { inclusive = true }
            launchSingleTop = true
        }
    }

    Column(
        modifier = modifier.fillMaxSize(),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(
            text = "Success!",
            style = MaterialTheme.typography.displayLarge,
            modifier = Modifier.padding(bottom = 16.dp)
        )

        LottieAnimation(
            composition = composition,
            progress = progress,
            modifier = Modifier
                .fillMaxWidth()
                .height(300.dp)
                .padding(16.dp),
            contentScale = ContentScale.Fit
        )
    }
}

// Entered when failure to send updated threshold settings to ESP32
@Composable
fun FailureScreen(
    navController: NavController,
    onResetTabIndex: () -> Unit,
    modifier: Modifier = Modifier
) {
    LaunchedEffect(Unit) {
        delay(3000)
        onResetTabIndex()
        navController.navigate(BiomeScreens.CURRENT_VALUES.name) {
            popUpTo(BiomeScreens.CURRENT_VALUES.name) { inclusive = true }
            launchSingleTop = true
        }
    }

    Column(
        modifier = modifier.fillMaxSize(),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(
            text = "Failed to update settings",
            style = MaterialTheme.typography.displayLarge,
            color = MaterialTheme.colorScheme.error,
            textAlign = TextAlign.Center,
            modifier = Modifier
                .padding(horizontal = 16.dp)
                .fillMaxWidth()
        )
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun BiomeTopAppBar(scrollBehavior: TopAppBarScrollBehavior) {
    CenterAlignedTopAppBar(
        title = { Text("Orchid Biome", color = MaterialTheme.colorScheme.background) },
        scrollBehavior = scrollBehavior,
        colors = TopAppBarDefaults.centerAlignedTopAppBarColors(
            containerColor = MaterialTheme.colorScheme.primary
        ),
        navigationIcon = {}
    )
}

// Displays while trying to connect to ESP32
@Composable
fun LoadingScreen(modifier: Modifier = Modifier) {
    Box(
        modifier = modifier.fillMaxSize(),
        contentAlignment = Alignment.Center
    ) {
        AsyncImage(
            model = ImageRequest.Builder(LocalContext.current)
                .data(R.raw.flower)
                .decoderFactory(ImageDecoderDecoder.Factory())
                .build(),
            contentDescription = null,
            modifier = Modifier
                .fillMaxWidth()
                .height(300.dp)
                .padding(16.dp),
            contentScale = ContentScale.Fit
        )
    }
}

// Displays when an error occurs on attempted connection
@Composable
fun ErrorScreen(message: String?) {
    Box(modifier = Modifier.fillMaxSize(), contentAlignment = Alignment.Center) {
        Text(
            text = message ?: "An unknown error occurred",
            color = MaterialTheme.colorScheme.error)
    }
}

// Displays when no connection is made after five seconds of attempting
@Composable
fun TimeoutScreen(
    viewModel: BiomeViewModel,
    modifier: Modifier = Modifier
) {
    Box(
        modifier = modifier.fillMaxSize(),
        contentAlignment = Alignment.Center
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Text(
                text = "Connection timeout",
                color = MaterialTheme.colorScheme.secondary,
                fontSize = 30.sp,
                fontWeight = FontWeight.Bold,
                modifier = Modifier.padding(16.dp)
            )
            Button(
                onClick = { viewModel.updateDataFromESP32() },
                colors = ButtonDefaults.buttonColors(containerColor = MaterialTheme.colorScheme.primary),
                modifier = Modifier.padding(top = 16.dp).width(200.dp)
            ) {
                Text("Retry")
            }
        }
    }
}