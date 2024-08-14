package com.example.myfirstapp

import android.app.DatePickerDialog
import android.icu.text.SimpleDateFormat
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import java.util.*

class MainActivity : AppCompatActivity() {

    private var tvSelectedDate : TextView ? = null
    private var tvAge: TextView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val btnSelectDate : Button = findViewById((R.id.btnSelectDate))
        tvSelectedDate = findViewById((R.id.txnSelectedDate))
        tvAge = findViewById((R.id.txnAge))
        btnSelectDate.setOnClickListener {
            clickSelectDate()
        }
    }

    private fun clickSelectDate(){
        val myCalendar = Calendar.getInstance()
        val year = myCalendar.get(Calendar.YEAR)
        val month = myCalendar.get(Calendar.MONTH)
        val day = myCalendar.get(Calendar.DAY_OF_MONTH)
        DatePickerDialog(this,
                        DatePickerDialog.OnDateSetListener{view, year, month, dayOfMonth ->
                            Toast.makeText(this,
                                "btnSelectDate pressed, year is ${year}, ${month+1}, day: ${dayOfMonth}",
                                Toast.LENGTH_LONG).show()

                            val selectedDate = "${dayOfMonth}/${month+1}/${year}"
                            tvSelectedDate?.text = selectedDate

                            val sdf = SimpleDateFormat("dd/MM/YYYY", Locale.ENGLISH)
                            val theDate = sdf.parse(selectedDate)

                            val age = (theDate.time / (525600* 60000)) - (Calendar.getInstance().get(Calendar.YEAR)/(525600* 60000))
                            val ageResult = "${age}"
                            tvAge?.text = ageResult

                            Toast.makeText(this,
                                "result age is  ${ageResult}",
                                Toast.LENGTH_LONG).show()
                        },
            year,
            month,
            day
            ).show()

    }
}