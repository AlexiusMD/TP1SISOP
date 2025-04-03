# Dynamic Process Execution Simulator - SISOP TP1

This repository contains the implementation of the first project (TP1) for the **Operating Systems (SISOP)** course, taught by **Professor Fabiano Hessel** at PUCRS.

## 🧠 Objective

This project simulates a dynamic environment for **process execution and scheduling**, where multiple tasks written in a **hypothetical assembly language** are managed and executed using the **Earliest Deadline First (EDF)** scheduling policy.

## 👥 Group Members

- Alexius Dias  
- João Carvalho  
- Pedro Souza  
- Rodrigo Sandler

## 🖥️ Project Overview

The system simulates:

- A CPU with **two registers**: an **accumulator (acc)** and a **program counter (pc)**.
- Execution of tasks written in a **custom assembly-like language**.
- **Dynamic scheduling** using the **EDF** algorithm.
- **System calls** for input/output and program termination.
- Process **preemption**, **blocking**, and **deadline tracking**.

Each task is composed of:
- An assembly code section (`.code ... .endcode`)
- A data section (`.data ... .enddata`)

### ⌨️ Supported Instructions

| Category     | Mnemonic     | Description |
|--------------|--------------|-------------|
| Arithmetic   | `ADD`, `SUB`, `MULT`, `DIV` | Operates on the accumulator (`acc`) |
| Memory       | `LOAD`, `STORE`            | Loads from or stores to memory |
| Branching    | `BRANY`, `BRPOS`, `BRZERO`, `BRNEG` | Conditional and unconditional jumps |
| System       | `SYSCALL`                  | System operations: 0 = halt, 1 = print, 2 = input (blocks for 1–3 cycles) |

📌 Use `#` before operands for **immediate mode**.  
Example: `SUB #1` subtracts 1 directly from the accumulator.

### 🧮 Example Program

```asm
.code
LOAD variable      # acc = variable
ponto1: SUB #1     # acc = acc - 1
SYSCALL 1          # print(acc)
BRPOS ponto1       # if acc > 0 → goto ponto1
SYSCALL 0          # end
.endcode

.data
variable 3         # initial value = 3
.enddata
```

### 🗂️ File Structure

```
/src        → Main source code files (.c)
/include    → Header files (.h)
/build      → Compiled output
README.md   → Project description
.gitignore  → Git exclusions
```

## 🧭 EDF Scheduling

- **EDF (Earliest Deadline First)** assigns dynamic priority based on the closest deadline.
- Tasks are **periodic** and have known **computation times**.
- When a higher-priority task arrives or a syscall is made, the current task is preempted.
- Tasks resume from where they stopped (context switching time is ignored).

### Task States

- **Running**: actively using the CPU.
- **Ready**: waiting to be scheduled.
- **Blocked**: waiting due to I/O syscall (for 1–3 time units).
- **Finished**: completed execution or halted.

## ⚙️ How to Compile and Run

### 🔧 Compilation

```bash
gcc -o simulator src/*.c
```

### ▶️ Execution

```bash
./simulator
```

(Instructions for input formats and parameters go here.)

## 📅 Delivery Info

- **Deadline:** April 22, 2025 – 17:15
- **Presentation:** April 22 or 24 via Moodle
- **File Format:** `.zip` file with the full group names in the filename
- **Submission:** Must include source code + user manual PDF

---

🛑 *Plagiarism will result in a zero. Projects with compilation errors will not be evaluated.*
