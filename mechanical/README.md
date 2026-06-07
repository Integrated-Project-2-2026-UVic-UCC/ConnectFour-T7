# Connect Four Robot – Mechanical Design

## Project Description
This repository contains the mechanical design of a robot capable of playing the Connect Four game. The mechanical system provides the physical structure of the game board and the mechanism responsible for storing, positioning, and dropping the playing pieces into the correct column.

The design is organized into two main assemblies: the board structure, which holds the playing grid, and the dispenser and linear-motion assembly, which moves along the board and releases the pieces one by one.

## Project Structure
During the development process, the mechanical design was divided into two main assemblies:
1. Board structure assembly
2. Dispenser and linear-motion assembly

Each part was modeled in PTC Creo, exported as an STL file for 3D printing, and documented with its corresponding technical drawing.

## Board Structure Assembly
This assembly represents the game board and provides the supporting frame of the robot. It is composed of:
* **Foot L** and **Foot R** – the two legs that hold the structure upright
* **Bottom Cover** – the lower cover that closes the structure and is assembled onto the base
* **Connect 4** – the main board frame containing the 6×7 grid of columns
* **Connect 4 Cover** – the front cover that closes the playing grid

The two feet and the bottom cover are assembled together to form the base, onto which the board frame and its cover are mounted.

## Dispenser and Linear-Motion Assembly
The second assembly is mounted on its own support and is responsible for carrying and releasing the pieces. It includes:
* **Support Foot** – the additional leg of this assembly
* **Dispenser Support Bar** – the bar that links the support and holds the dispenser in place
* **Motor Support** – the bracket that holds the stepper motor at one end of the lead screw
* **Bearing Support** – the bracket that holds the bearing at the opposite end of the lead screw
* **Dispenser Part 1**, **Dispenser Part 2** and the **Dispenser Support Bar** – the parts that form the dispenser body

A support element attaches the carriage and its bushing to the dispenser, allowing the whole dispenser to slide along the lead screw.

## Piece Dispensing Mechanism
The pieces are stored horizontally inside the dispenser. To drop a piece, the dispenser uses an internal ramp that reorients the piece from a horizontal to a vertical position, so that it can fall correctly into the selected column.

A servo motor mounted on the dispenser pushes the ramp downward, releasing one piece at a time.

## Linear-Motion System
The dispenser moves horizontally along the board by means of a lead screw (worm screw):
* The **Motor Support** holds the stepper motor at the beginning of the lead screw
* The **Bearing Support** holds the bearing at the end of the lead screw
* The carriage, attached to the dispenser, travels along the screw to position the dispenser over the desired column

This allows the robot to align the dispenser precisely with any of the seven columns before releasing a piece.

## File Formats and Repository Structure
For each part, three file formats are provided. The files are organized into three folders:
* **STL_3D_Parts** – the 3D models in STL format, ready for 3D printing
* **Creo_Files** – the original PTC Creo part files (`.prt`), including the Creo session trail file
* **Technical_Drawings** – the technical drawings (blueprints) of each part in PDF format

## Current Project Status
The mechanical design has reached a functional state where:
* The board structure and base are fully modeled and assembled
* The dispenser and its ramp mechanism convert pieces from horizontal to vertical for correct release
* The servo-driven dropping mechanism has been integrated into the dispenser
* The linear-motion system positions the dispenser over each column using a lead screw
* All parts are documented with STL models, Creo files, and technical drawings ready for manufacturing
