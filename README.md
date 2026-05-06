# 🍅 Low-Cost Edge AI System for Tomato Ripeness & Quality Classification

![Platform](https://img.shields.io/badge/platform-ESP32--S-blue)
![Framework](https://img.shields.io/badge/framework-Edge%20Impulse-orange)
![Model](https://img.shields.io/badge/model-MobileNetV2%2096x96%200.35-green)
![Accuracy](https://img.shields.io/badge/test%20accuracy-84.9%25-brightgreen)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

> An embedded deep-learning system that classifies tomatoes as **ripe**, **unripe**, or **rotten**, and communicates the result through an RGB LED-running on a microcontroller small enough to fit inside a child's toy lamp.

<p align="center">
  <img src="Media/Enclosure + Components/[Picture13].png" width="320" alt="Assembled prototype glowing green for a ripe tomato"/>
  <br/>
  <em>The assembled prototype signalling “ripe” in green. Amber = unripe, red = rotten.</em>
</p>

---

## 📑 Table of Contents

1. [Overview](#-overview)
2. [Motivation](#-motivation)
3. [Target Users](#-target-users)
4. [System Architecture](#-system-architecture)
5. [Hardware & Components](#-hardware--components)
6. [Wiring](#-wiring)
7. [Software Stack](#-software-stack)
8. [Dataset](#-dataset)
9. [Neural Network & Training](#-neural-network--training)
10. [Classification & On-Device Behaviour](#-classification--on-device-behaviour)
11. [Results](#-results)
12. [Demo](#-demo)
13. [Repository Structure](#-repository-structure)
14. [Quick Start](#-quick-start)
15. [Lessons Learned](#-lessons-learned)
16. [Future Improvements](#-future-improvements)
17. [Acknowledgements & References](#-acknowledgements--references)

---

## 📌 Overview

This project is a **low-cost embedded AI system** that classifies tomato ripeness and quality from images and provides **real-time visual feedback** through an RGB LED interface. Instead of dashboards or numbers, the device speaks in colour:

| Class | LED Colour | Meaning |
|---|---|---|
| 🟢 **Ripe** | Green | Sellable / ready to eat |
| 🟡 **Unripe** | Amber | Not ready - leave to ripen |
| 🔴 **Rotten** | Red | Damaged / unsellable |

The model runs on an **ESP32-S** microcontroller using a transfer-learned **MobileNetV2** trained in **Edge Impulse**, and drives an **8-pixel NeoPixel RGB stick** for the visual output. Total bill of materials: under £20.

---

## 🎯 Motivation

Globally, more than 13% of food is lost between farm and retail every year, and fruit and vegetables are among the worst-affected categories — losses for fresh produce can exceed 25% of total production [(FAO, 2024)](https://www.fao.org/policy-support/policy-themes/food-loss-and-food-waste/fao-policy-series--food-loss---food-waste). A major driver is poor or inconsistent ripeness sorting at the point of harvest and sale.

Manual sorting is:

- ⏱️ **Time-consuming**
- 🎲 **Inconsistent** between graders
- 🧑‍🔬 **Dependent on individual judgement**

Industrial computer-vision sorters exist but cost tens of thousands of pounds, putting them out of reach of smallholders, market vendors, and home growers. This project asks a deliberately simple question:

> **Can the same idea — image-based ripeness classification — be delivered for under £20, on a microcontroller, with a non-technical visual interface?**

---

## 👥 Target Users

- 🌾 **Smallholder farmers** sorting harvests
- 🛒 **Market vendors** displaying produce
- 🚚 **Small-scale food distributors** doing quality checks
- 🌱 **Home growers** monitoring their own crops

The colour-coded interface means the device is usable across language barriers and by users without technical training.

---

## 🧠 System Architecture

```text
┌───────────────┐    ┌───────────────┐    ┌──────────────┐    ┌─────────┐    ┌────────────┐
│ Laptop webcam │ →  │ Edge Impulse  │ →  │ Class label  │ →  │ ESP32-S │ →  │ NeoPixel   │
│  (frame in)   │    │ MobileNetV2   │    │ + confidence │    │  (USB   │    │  (RGB out) │
│               │    │  inference    │    │   over USB   │    │  serial)│    │            │
└───────────────┘    └───────────────┘    └──────────────┘    └─────────┘    └────────────┘
```

A frame is captured from the laptop webcam, classified by the on-device-ready Edge Impulse model, and the resulting label is sent over USB serial to the ESP32-S. The microcontroller drives the NeoPixel with the colour mapped to that class.

> **Note on the camera.** The ESP32-S board used in this build does not include a camera, so the laptop webcam was used as the image source for prototyping. The exact same Edge Impulse Arduino library can be deployed to a camera-equipped board (e.g. **ESP32-CAM**) without changes to the model or the LED logic - see [Future Improvements](#-future-improvements).

---

## 🔧 Hardware & Components

<p align="center">
  <img src="Media/Enclosure + Components/IMG_7381.jpeg" width="640" alt="ESP32S board and NeoPixel stick laid out before assembly"/>
  <br/>
  <em>Core components: <strong>Adafruit NeoPixel Stick</strong> (left) and <strong>ESP32-S</strong> dev board (right).</em>
</p>

| Component | Role | Approximate cost |
|---|---|---|
| ESP32-S dev board | Microcontroller running the inference + LED driver | ~£8 |
| Adafruit NeoPixel Stick (8 × WS2812 RGB LEDs) | Visual output | ~£5 |
| Repurposed LED toy lamp | Enclosure (upcycled — no 3D printing required) | £0 |
| Jumper wires (3) | VCC, GND, DATA | <£1 |
| USB cable | Power + serial link to laptop | already on hand |
| **Total** | | **≈ £14–£15** |

<p align="center">
  <img src="Media/Enclosure + Components/IMG_7374.jpeg" width="380" alt="ESP32S seated inside the green LED-toy-lamp enclosure"/>
  <br/>
  <em>The ESP32-S seated inside the upcycled LED toy-lamp enclosure during assembly.</em>
</p>

---

## 🔌 Wiring

Only three wires are needed between the NeoPixel stick and the ESP32-S:

| NeoPixel Stick | ESP32-S | Notes |
|---|---|---|
| `GND` | `GND` | Common ground |
| `VCC` | `5V / VIN` | Powered from USB rail |
| `DIN` | `GPIO 5` | Single-wire WS2812 data line |

<p align="center">
  <img src="Media/ESP32S + NeoPixel (WS2812) Wiring/[IMG_7382.jpeg]" width="300" alt="NeoPixel wired to ESP32-S during assembly"/>
  <br/>
  <em>Wiring during assembly - green = data, red = 5 V, ground shared.</em>
</p>

---

## 💻 Software Stack

- **Edge Impulse Studio** - data acquisition, transfer-learning training, EON Tuner
- **TensorFlow / Keras** (under the hood, via Edge Impulse) — model training
- **TensorBoard** - accuracy and loss visualisation
- **Arduino IDE** with the **ESP32 board package** — flashing the microcontroller
- **Adafruit NeoPixel** library - driving the RGB LEDs
- **Edge Impulse Arduino library** (auto-exported from Studio) — on-device inference

---

## 📊 Dataset

The training dataset combined publicly available tomato images:

- **Ripe** and **unripe** classes -  drawn from the [Riped and Unripe Tomato Dataset](https://www.kaggle.com/datasets) on Kaggle
- **Rotten** class - sourced from the [Fruits-360 dataset](https://www.kaggle.com/datasets/moltean/fruits)

After cleaning and balancing, the working dataset contained roughly **300 labelled images**, split **80 / 20** between training and validation in Edge Impulse. Each image was resized to **96 × 96 RGB** (yielding the **27,648 input features** seen in the impulse design), normalised, and augmented at training time with random flips, brightness shifts, and crops to reduce overfitting on a small dataset.

---

## 🧠 Neural Network & Training

The model is a transfer-learned **MobileNetV2 (96 × 96, width multiplier 0.35)** with ImageNet-pretrained weights, a **16-neuron dense head with 0.1 dropout**, and a **3-class softmax output**. MobileNetV2's inverted-residual bottlenecks are designed for mobile and embedded deployment - they give an excellent accuracy-to-footprint ratio on resource-constrained hardware.

<p align="center">
  <img src="Media/Edge Impulse Screenshot/[Picture12].png" width="640" alt="Edge Impulse impulse design and on-device performance"/>
  <br/>
  <em>Impulse design (left): Input 27,648 features → MobileNetV2 96×96 0.35 → dense (16, dropout 0.1) → softmax (3). Right: feature embedding and on-device performance.</em>
</p>

**Training configuration**

| Parameter | Value |
|---|---|
| Optimiser | Adam |
| Learning rate | 0.0005 |
| Batch size | 32 |
| Epochs | 40 |
| Validation split | 20 % |
| Auto-weight classes | enabled |
| Quantisation profiling | int8 enabled |
| Data augmentation | flip + brightness + crop |

<p align="center">
  <img src="Media/Edge Impulse Screenshot/[Picture6].png" width="640" alt="Edge Impulse training accuracy and loss curves"/>
  <br/>
  <em>Edge Impulse training graphs - epoch accuracy (top) and epoch loss (bottom). Curves converge by ~epoch 15 with no severe overfitting.</em>
</p>

**On-device profile** (target: Cortex-M4F @ 80 MHz, EON Compiler RAM-optimised)

| Metric | Value |
|---|---|
| Inference time per frame | **1,045 ms** |
| Peak RAM | **232.9 KB** |
| Flash usage | **546.0 KB** |

---

## 🟢🟡🔴 Classification & On-Device Behaviour

At runtime the laptop captures a webcam frame, the Edge Impulse model returns a class label (and probabilities), and the result is written over USB serial to the ESP32-S. The microcontroller parses the string and sets the NeoPixel colour. A simplified extract of the ESP32-S sketch:

```cpp
void loop() {
  if (Serial.available()) {
    String prediction = Serial.readStringUntil('\n');
    prediction.trim();
    prediction.toLowerCase();

    if      (prediction == "ripe")    setColor(  0, 255,   0);  // green
    else if (prediction == "unripe")  setColor(255, 120,   0);  // amber
    else if (prediction == "rotten")  setColor(255,   0,   0);  // red
    else                              setColor(  0,   0, 255);  // blue = unknown / uncertain
  }
}
```

A fourth **"uncertain"** state is emitted whenever the model's top-class probability falls below **0.6** - this prevents confident misclassifications and is shown as blue on the LED. The same idea was added as an `UNCERTAIN` column in the test confusion matrix (see [Results](#-results)).

---

## 📈 Results

### Validation set (quantised int8 model)

<p align="center">
  <img src="Media/Edge Impulse Screenshot/[Picture1].png" width="420" alt="Edge Impulse validation confusion matrix at 84.9% accuracy"/>
</p>

- **Validation accuracy:** 84.9 %
- **Validation loss:** 0.41
- **F1 scores:** ripe 0.83, rotten 0.87, unripe 0.83

### Held-out test set (with UNCERTAIN class)

<p align="center">
  <Media/Edge Impulse Screenshot/[Picture8].png" width="640" alt="Test-set headline metrics and confusion matrix"/>
</p>

- **Test accuracy:** **84.93 %**
- **Area under ROC:** **0.98**
- **Weighted precision / recall / F1:** **0.92 / 0.92 / 0.92**
- **Per-class F1:** ripe 0.91, rotten 0.84, unripe 0.91

The strongest class is **unripe** (recall 91.3 %), followed by **ripe** (87.0 %); **rotten** is weakest (77.8 %), most often slipping into the **UNCERTAIN** bucket - a desirable failure mode, because the device simply withholds a recommendation rather than committing to a wrong answer.

### Feature embedding

<p align="center">
  <Media/Enclosure + Components/demo.mov" width="640" alt="Feature explorer scatter plot showing ripe/unripe/rotten clusters"/>
  <br/>
  <em>Feature Explorer - each dot is one sample. Ripe and unripe form distinct clusters; rotten partially overlaps with ripe (very ripe red and lightly rotting tomatoes share a colour palette).</em>
</p>

---

## 🎬 Demo

A short demo video (`demo.mov`) shows the full end-to-end pipeline: a tomato image is shown to the laptop webcam, the model classifies it, and the lamp glows green / amber / red in real time.

> 📹 **Watch:** [Watch the demo](Media/Enclosure-and-Components/demo.mov) in this repository.

---

## 📂 Repository Structure

```
.
├── README.md                       ← this file
├── arduino/
│   └── Tomato_AI_Project.ino       ← ESP32-S sketch (NeoPixel + serial parser)
├── edge_impulse/
│   ├── README.md                   ← link to public Edge Impulse project
│   └── exported_library/           ← Edge Impulse Arduino library (auto-generated)
├── inference_host/
│   └── classify.py                 ← laptop-side script: webcam → model → serial
├── readme_assets/                  ← images embedded in this README
└── docs/
    └── Tomato_Ripeness_Edge_AI_Report.docx   ← full coursework report
```

---

## 🚀 Quick Start

### 1. Flash the ESP32-S

```bash
# In the Arduino IDE:
# 1. Install ESP32 board support  (Boards Manager → "esp32")
# 2. Install Adafruit NeoPixel    (Library Manager → "Adafruit NeoPixel")
# 3. Open arduino/Tomato_AI_Project.ino
# 4. Select Board: "ESP32 Dev Module", set the correct COM port
# 5. Click Upload
```

### 2. Run the host-side classifier

```bash
git clone https://github.com/Madina1219/CASA0018-SmartTomatoSortingSystemUsingEdgeAIandVisualFeedback.git
cd CASA0018-SmartTomatoSortingSystemUsingEdgeAIandVisualFeedback/inference_host
pip install edge_impulse_linux opencv-python pyserial
python classify.py --port /dev/ttyUSB0      # macOS/Linux
# python classify.py --port COM3            # Windows
```

The script opens the webcam, sends each frame through the local Edge Impulse model, and writes the predicted label (`ripe` / `unripe` / `rotten`) to the serial port the ESP32 is listening on.

### 3. Point a tomato at the camera 🍅

The lamp should glow green for ripe, amber for unripe, red for rotten, and blue for uncertain.

---

## 📚 Lessons Learned

- **Data quality > model size.** Early models confused unripe and rotten constantly. Rebalancing the dataset and adding an explicit *uncertain* output bucket helped much more than swapping in a bigger backbone.
- **Quantisation matters more than expected.** The float32 test accuracy was 84.9 %, but the int8 test accuracy dropped to 68.5 % - a wider gap than I anticipated. Quantisation-aware training (rather than post-hoc quantisation) would close most of that gap.
- **Visual output beats numbers.** Non-technical users do not need to see a confidence score. They see green, and they know.
- **No on-board camera = not truly edge.** Using the laptop webcam was a valid prototyping shortcut, but the system isn't fully embedded until both the model **and** the camera live on the device.
- **Upcycled enclosures are underrated.** A LED toy lamp with a hollow body is, mechanically, an almost-perfect microcontroller enclosure: light-diffusing, robust, and free.

---

## 🔮 Future Improvements

| Priority | Improvement | Why |
|---|---|---|
| ⭐⭐⭐ | Migrate to **ESP32-CAM** | Removes the laptop dependency — fully on-device pipeline |
| ⭐⭐⭐ | **Quantisation-aware training** | Recovers the int8 accuracy gap (currently ~16 pp) |
| ⭐⭐ | Field-collected dataset under **natural / variable lighting** | Bridges the gap between studio-clean training images and real kitchen / market lighting |
| ⭐⭐ | Add a **buzzer for accessibility** | Makes the device usable by visually-impaired users |
| ⭐ | **Battery + 3D-printed case** | Truly portable in-field unit |
| ⭐ | Extend to **other produce** (peppers, mangoes, avocados) | Same pipeline; only the dataset and labels change |
| ⭐ | Stream classifications to a phone via **BLE** | Optional analytics dashboard for vendors |

---

## 🙏 Acknowledgements & References

- **MobileNetV2 architecture:** Sandler, M., Howard, A., Zhu, M., Zhmoginov, A. and Chen, L.-C. (2018). *MobileNetV2: Inverted Residuals and Linear Bottlenecks.* CVPR.
- **TinyML deployment patterns:** Warden, P. and Situnayake, D. (2019). *TinyML.* O'Reilly Media.
- **Edge Impulse** — image classification tutorials and on-device deployment tools. <https://docs.edgeimpulse.com>
- **Food loss statistics:** FAO (2024). *The State of Food and Agriculture 2024.* <https://www.fao.org/policy-support/policy-themes/food-loss-and-food-waste/fao-policy-series--food-loss---food-waste>
- **Datasets:** Kaggle *Riped and Unripe Tomato Dataset*; Mureșan, H. & Oltean, M. (2018) *Fruits-360.*
- **Module:** Built as part of the **CASA0018 — Deep Learning for Sensor Networks** module, UCL.

---

## 📜 License

This project is released under the **MIT License**. See [`LICENSE`](LICENSE) for details.

---

<p align="center">
  <em>Built with 🍅, an ESP32, and an old toy lamp.</em>
</p>
t
