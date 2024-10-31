package com.example.myid

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import com.example.myid.databinding.FragmentUpdatePageBinding
import com.example.myid.viewmodel.EmployeeInfoViewModel

class UpdatePageFragment : Fragment() {

    private var _binding: FragmentUpdatePageBinding? = null
    private val binding get() = _binding!!
    private val sharedViewModel: EmployeeInfoViewModel by activityViewModels()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentUpdatePageBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.buttonUpdate.setOnClickListener {
            val name = binding.editTextName.text.toString()
            val department = binding.editTextDepartment.text.toString()
            val employeeId = binding.editTextEmployeeId.text.toString()

            sharedViewModel.updateName(name)
            sharedViewModel.updateDepartment(department)
            sharedViewModel.updateEmployeeId(employeeId)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
