# 🍅 Low-Cost Edge AI System for Tomato Ripeness & Quality Classification

## 📌 Overview
This project presents a **low-cost embedded AI system** that classifies tomato ripeness and quality using image-based deep learning and provides **real-time visual feedback** through an RGB LED interface.

The system categorises tomatoes into three classes:

- 🟢 **Ripe (Sellable)**
- 🟡 **Unripe (Not ready)**
- 🔴 **Rotten/Damaged (Unsellable)**

Rather than displaying complex numerical outputs, the system translates predictions into **intuitive colour signals**, enabling fast and accessible decision-making for non-technical users.

---

## 🎯 Motivation
In agricultural and market environments, manual sorting of produce is:

- Time-consuming  
- Inconsistent  
- Dependent on human judgement  

This project explores how **embedded AI** can support:

- Faster sorting  
- Reduced waste  
- Improved quality control  

> **Goal:** Design a system that is simple, low-cost, and easy to interpret in real-world conditions.

---

## 👥 Target Users
- Farmers  
- Market vendors  
- Small-scale food distributors  

These users benefit from **instant visual feedback** without needing to interpret data or interact with complex interfaces.

---

## 🧠 System Architecture
```text
Tomato Image → CNN Model → Classification → ESP32 → RGB LED Output
