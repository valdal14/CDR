# C-Data Reconciler (CDR)

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white) ![GCC](https://img.shields.io/badge/Compiler-GCC%20%7C%20Clang-orange?style=flat) ![License](https://img.shields.io/badge/License-MIT-green?style=flat)

A high-performance, memory-safe data reconciliation engine written in pure C. CDR is designed to ingest, parse, and mathematically compare massive dual CSV data streams to detect discrepancies at the memory level. 

By avoiding heavy heap allocations and utilizing stack-based Data Transfer Objects, CDR can process tens of thousands of lines per second, generating granular forensic audit logs for enterprise data pipelines.

## Features
* **Dual-Mode Orchestrator:** Choose between Auto-Inference for frictionless comparison or Manual Schema mapping for strict memory blueprinting.
* **Smart Inference Engine:** Dynamically deduces `INT`, `FLOAT`, and `STRING` (Max 64 chars) memory types directly from raw strings.
* **Non-Destructive Auditing:** Safely preserves raw string buffers prior to `strtok` mutation to write exact mismatch evidence to disk.
* **Memory Safety:** Strictly isolates stack frames and manages file descriptor lifecycles to guarantee zero memory leaks across concurrent executions.

---

## Build Instructions

Ensure you have a standard C compiler (e.g., `gcc` or `clang`) and `make` installed.

1. Clone the repository:
   `git clone git@github.com:valdal14/CDR.git`
2. Navigate to the directory:
   `cd CDR`
3. Compile the binary:
   `make`
4. Execute the engine:
   `./cdr_engine`

---

## Workflows

The CDR Engine supports two distinct reconciliation workflows based on your data structure. Upon launching the engine, type `AUTO` or `MANUAL` at the `CDR>` prompt.

### 1. Auto-Inference Mode (AUTO)
Best for standard datasets that include a Header row. The engine will extract column names from Row 1, dynamically infer memory types from Row 2, and execute the comparison in a single pass.
* **Prompt:** `Enter File A, File B, and Separator (space separated):`
* **Input Example:** `bucket_a.csv bucket_b.csv ,`

### 2. Manual Schema Mode (MANUAL)
Best for headless data dumps (no headers) or when strict type enforcement is required. You must define the memory blueprint manually via the CLI before passing the files.
* **Prompt:** `CDR 📄 schema>`
* **Command:** `ADD COL (Column Name) TYPE`
* **Supported Types:** `INT`, `FLOAT`, `STRING`
* **Input Example 1:** `ADD COL (Transaction ID) INT`
* **Input Example 2:** `ADD COL (Amount) FLOAT`
* **Execution:** Type `RUN` when the schema is complete.

---

## The Audit Log

Upon completion, CDR will print a statistical summary to the terminal. If mismatches are detected, a physical `reconciliation_report.log` file is generated in the root directory. This file contains the exact line numbers and the unmutated string values from both files, allowing engineers to track down truncation, locale shifts, and encoding errors.

---

## Contributing

We welcome contributions from Systems Engineers and Data Architects. To contribute to the CDR project:

1. Fork the repository.
2. Create a new feature branch (`git checkout -b feature/your-feature-name`).
3. Ensure all new functions are thoroughly documented with Doxygen-style docstrings.
4. Maintain strict memory safety (prefer stack allocation, ensure `malloc` calls have corresponding `free` calls).
5. Submit a Pull Request detailing the architectural changes and the problem solved.
