import 'package:flutter/material.dart';
import 'NavigationApp.dart';

class IdPage extends StatelessWidget {
  const IdPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold (
      // backgroundColor: Colors.cyan[900],
      // appBar: AppBar (
      //   title: const Text ('TMA Employee',
      //     style: TextStyle(
      //       color: Colors.white
      //     ),
      //   ),
      //   centerTitle: true,
      //   backgroundColor: Colors.lightBlue[900],
      // ),
      bottomNavigationBar: const NavigationBarApp()
    );
  }
}