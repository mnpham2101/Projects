package com.example.calculator

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Button
import android.widget.TextView
import android.widget.Toast

class MainActivity : AppCompatActivity() {

    private var resultView:TextView ?= null;

    private var isDecimal = false
    private var leadingZero = true
    private var aNumber = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        resultView = findViewById(R.id.resultView)
    }

    fun onDigit (view: View){
        resultView?.append((view as Button).text)
        leadingZero = false
        aNumber = true
    }

    fun onClear (view: View){
        resultView?.text = ""
        resetNumber()
    }

    fun onDecimal (view: View){
        if (!isDecimal){
            resultView?.append((view as Button).text)
            isDecimal = true
            leadingZero = false
            aNumber = true
        }
    }

    fun onZero (view:View){
        if (!leadingZero){
            resultView?.append((view as Button).text)
            leadingZero = false
        }
    }

    fun onOperation(view: View){
        if (aNumber){
            resultView?.append((view as Button).text)
            resetNumber()
        }
    }

    private fun resetNumber(){
        isDecimal = false
        leadingZero = true
        aNumber = false
    }
}
/*  Todos:
* basic: enforce 1 operation, must have equal sign after onOperation
* => can also implement 2 result view, 1 with full operations, the other with just the number like window calculator
* advance: enforce multiple operation, must perform operation precedence
* allow second number to be zero, but not allow sth like 0123
* get substring that includes only number or operation.
* */