package com.example.myid

import android.os.Bundle
import android.util.Log
import android.view.KeyEvent
import androidx.activity.OnBackPressedCallback
import androidx.appcompat.app.AppCompatActivity
import com.example.myid.databinding.ActivityMainBinding
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentTransaction
import androidx.navigation.NavController
import androidx.navigation.NavDestination
import androidx.navigation.findNavController
import androidx.navigation.fragment.NavHostFragment
import androidx.navigation.ui.setupWithNavController

class MainActivity : AppCompatActivity() {
    // Step 1: Generated binding class
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // Step 1: Generated binding class
        binding = ActivityMainBinding.inflate(layoutInflater)
        // Step 2: Get a reference to the root view
        val view = binding.root
        // Step 3: Pass the root view
        setContentView(view)

        // Add the fragment to the activity using the fragment manager and a transaction
//        supportFragmentManager.beginTransaction().apply {
//            add(R.id.fragment_home_page, HomePageFragment())
//            commit()
//        }

        // get a NavHostFragment, and then retrieve the NavController
        // NavHostFragment provides fragment_home_page
        val navHostFragment = supportFragmentManager.findFragmentById(R.id.nav_host_fragment)
                as NavHostFragment
        val navController = navHostFragment.navController

        // Set up BottomNavigationView with NavController
        binding.bottomNavigation.setupWithNavController(navController)

        navController.addOnDestinationChangedListener { controller, destination, arguments ->

            Log.d("TEST111", "getDestination ${destination} ${controller.currentBackStackEntry}")
        }
        // Override the back button behavior
        onBackPressedDispatcher.addCallback(this, object : OnBackPressedCallback(true) {
            override fun handleOnBackPressed() {
                if (!navController.popBackStack()) {
                    finish()
                } // If no fragments in the back stack, finish the activity } } })
            }
        })
    }

//    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
//        if ((keyCode == KeyEvent.KEYCODE_BACK)) {
//            try {
//                binding.navHostFragment.findNavController().popBackStack()
//            } catch (e: Exception) {
//                e.printStackTrace()
//            }
//        }
//        return super.onKeyDown(keyCode, event)
//    }
}

