package com.example.myid.viewmodel

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

class EmployeeInfoViewModel : ViewModel() {
    private val _name = MutableLiveData<String>()
    val name: LiveData<String> get() = _name

    private val _department = MutableLiveData<String>()
    val department: LiveData<String> get() = _department

    private val _employeeId = MutableLiveData<String>()
    val employeeId: LiveData<String> get() = _employeeId

    fun updateName(newName: String) {
        _name.value = newName
    }

    fun updateDepartment(newDepartment: String) {
        _department.value = newDepartment
    }

    fun updateEmployeeId(newEmployeeId: String) {
        _employeeId.value = newEmployeeId
    }
}