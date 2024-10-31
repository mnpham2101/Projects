package com.example.myid

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import com.example.myid.databinding.FragmentHomePageBinding
import com.example.myid.viewmodel.EmployeeInfoViewModel

class HomePageFragment : Fragment() {

    private var _binding: FragmentHomePageBinding? = null
    private val binding get() = _binding!!
    private val employeeInfoViewModel: EmployeeInfoViewModel by activityViewModels()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentHomePageBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        employeeInfoViewModel.name.observe(viewLifecycleOwner) { name ->
            binding.textViewName.text = "Name: $name"
        }

        employeeInfoViewModel.department.observe(viewLifecycleOwner) { department ->
            binding.textViewDepartment.text = "Department: $department"
        }

        employeeInfoViewModel.employeeId.observe(viewLifecycleOwner) { employeeId ->
            binding.textViewEmployeeId.text = "Employee ID: $employeeId"
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}