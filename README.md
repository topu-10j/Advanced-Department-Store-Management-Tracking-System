# 🏬 Advanced Department Store Management & Tracking System

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![C](https://img.shields.io/badge/Language-C-blue.svg)]()
[![Version](https://img.shields.io/badge/Version-1.0-red.svg)]()

## 📌 Overview
A complete department store management system implemented in C using various Data Structures.

## ✨ Features

### 🏷️ Product Management
- Add/Edit/Delete Products
- Search by ID or Name
- View All Products

### 📦 Inventory Management
- Current Stock Status
- Low Stock Alert (Threshold: 10)
- Expiry Date Tracking
- Reorder Management

### 👥 Employee Management
- Add/Remove Employees
- Attendance Tracking
- Salary Management (with bonus)
- Shift Scheduling

### 🛒 Sales & Billing
- New Bill Generation (Queue System)
- View All Bills/Invoices
- Return/Exchange (Stack System)
- Daily Sales Report

### 👤 Customer Management
- Customer Registration
- Loyalty Points System
- Purchase History
- VIP Customer List

### 📊 Reports & Analytics
- Best Selling Products
- Revenue Analysis
- Department-wise Sales
- Peak Hours Analysis

### 🗺️ Store Navigation
- Shortest Path to Product (Dijkstra's Algorithm)
- Department Layout
- Staff Movement Optimization

### ⚙️ Settings
- System Configuration
- Backup/Restore Data
- User Permissions
- Tax & Discount Settings

## 📊 Data Structures Used

| Structure | Purpose |
|-----------|---------|
| Array | Store products, employees, bills |
| Queue | Billing queue (FIFO) |
| Stack | Return management (LIFO) |
| Graph | Store navigation (Dijkstra) |
| Struct | Product, Employee, Bill |

## 🚀 How to Run

```bash
# Compile
gcc store.c product.c inventory.c employee.c sales.c customer.c reports.c navigation.c settings.c utils.c -o store

# Run
./store
