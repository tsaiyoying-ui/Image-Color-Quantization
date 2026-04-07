# Image Color Quantization Tool

## Proposal Report

### Motivation and Goal
High-resolution 24-bit True Color images ($16.7$ million colors) consume significant memory and bandwidth. The goal of this project is to implement the Octree algorithm to compress image colors to a target number (e.g., 256). This approach significantly reduces data size while maintaining as much visual fidelity as possible through efficient color partitioning.

### Expected Functions
Image Parsing: Read and parse RGB data from standard lossless formats such as BMP or PPM.

Dynamic Octree Construction: Build an 8-level deep Octree based on the unique color distribution of the input image.

Color Pruning: Implement a pruning algorithm to automatically merge leaf nodes with the lowest frequencies.

Indexed Image Output: Generate the quantized image file along with its corresponding Color Palette.

Customization: Allow users to define the target number of colors (e.g., 16, 64, or 256 colors).

### Technologies Used
Language: C++ (chosen to demonstrate manual memory management and pointer manipulation).

Tools: VS Code, GCC, Git/GitHub (for version control and PR interaction).

Image Processing: Custom-written file I/O for header and pixel data parsing, avoiding heavy third-party libraries to focus on core logic.

### Timeline
Week 7: Submit proposal and complete the basic Image I/O module.

Weeks 8–9: Develop the Octree data structure class and node insertion logic.

Weeks 10–11: Implement recursive pruning and submit the Prototype Report.

Weeks 12–14: Refactor with a Priority Queue to optimize pruning efficiency and conduct performance testing.

Week 15: Record the Demo video and submit the Final Report.

### Relationship with the Course
This project directly applies several core concepts from the DSAP curriculum:

Data Structures (Octree): Utilizes an N-ary Tree to partition 3D RGB space, achieving $O(\log_8 N)$ complexity for color insertion and search.

Recursion: Employs recursive traversal for tree depth management, node merging, and average color calculation.

Priority Queue: Uses a Min-Heap to manage leaf node weights, ensuring that the least important colors are merged first to minimize visual distortion.

Advanced Programming: Demonstrates proficiency in dynamic memory management (new/delete) and Object-Oriented Design (OOD).

---

## Prototype Report

### 目前進度
<!-- 完成了什麼 -->

### 遇到的困難
<!-- 遇到什麼問題、如何解決或打算如何解決 -->

### 下一步計畫
<!-- 接下來要做什麼 -->

### 與課程的關聯
<!-- 到目前為止，你的實作中哪些部分與課程內容有關？關係是什麼？ -->

---

## Final Report

### 專案說明
<!-- 完整描述你的專案做了什麼 -->

### 使用方式
<!-- 如何編譯、執行、使用你的程式 -->

### 與課程的關聯總結
<!-- 總結你的專題與進階程式設計及資料結構課程之間的關聯 -->
