# 3D Scanner

## Overview

This project, created by Ashutosh Singh, is a Semester 4 Project Submission. It involves the use of two stepper motors for rotational and vertical motion, a Time-of-Flight (ToF) VL53L0X sensor, and an SD card module. The project aims to create a 3D scanner capable of generating a point cloud and subsequently a 3D model of an object by utilizing stored x, y, z values for each point on the object.

## Hardware Components

- Two stepper motors for rotational and vertical motion.
- Time-of-Flight (ToF) sensor - VL53L0X.
- SD card module.

## Software Requirements

- [MeshLab](https://www.meshlab.net/) - Software for processing and editing 3D triangular meshes.

## Project Workflow

### 1. Data Acquisition

- Stepper motors control the rotational and vertical movement.
- Time-of-Flight (ToF) sensor collects distance data for each point on the object.

### 2. Data Storage

- The collected x, y, z values are stored in an SD card.

### 3. Point Cloud Generation

- Utilizing the stored data, a point cloud is generated representing the object's surface.

### 4. Mesh Creation

- MeshLab is used to process the point cloud data and create a mesh.

### 5. Filter Techniques

- Various filtering techniques are applied to enhance the point cloud and refine the mesh.

### 6. 3D Model Generation

- The refined mesh is used to create a detailed 3D model of the scanned object.

## Repository Structure

```plaintext
/3D-Scanner
|-- /Documentation
|   |-- README.md
|   |-- Circuit_Diagram.png
|   |-- Project_Report.pdf
|-- /Code
|   |-- README.md
|   |-- stepper_motor_control.ino
|   |-- tof_sensor_data_collection.ino
|   |-- main.ino
|-- /Meshlab_Filters
|   |-- filter_script_1.mlx
|   |-- filter_script_2.mlx
|-- /3D_Models
|   |-- scanned_object.stl
|-- /Data
|   |-- data_log.txt
|-- README.md

