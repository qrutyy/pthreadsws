# Smokers Problem

## Overview

The **Smokers Problem** is a classic synchronization issue that demonstrates how multiple threads can coordinate access to shared resources. In this scenario, there are three smokers and an agent. Each smoker has an infinite supply of one ingredient: **tobacco**, **paper**, or **matches**. The agent places two of the three ingredients on the table, and the smoker with the missing ingredient picks them up to smoke.

## Purpose

This problem illustrates the challenges of thread synchronization, helping to prevent race conditions and deadlocks. The solution uses semaphores and mutexes to ensure that smokers and the agent can interact safely and efficiently.

Also, there are some limitations, Patil placed the following constraints on the cigarette smokers problem:

- The agent code is not modifiable.
- The solution is not allowed to use conditional statements.

[wiki](https://en.wikipedia.org/wiki/Cigarette_smokers_problem)

## How It Works

1. **Agents and Smokers**: 
   - The agent randomly places two ingredients on the table.
   - Each smoker waits for the right ingredients to smoke.

2. **Synchronization**:
   - A semaphore is used to signal when the agent can place items on the table.
   - Mutexes control access for each smoker to ensure they can only proceed when it's their turn.

## Running the Code
Check [Makefile](https://github.com/qrutyy/uthreadsworkshop/blob/feat/smokers/Makefile)

