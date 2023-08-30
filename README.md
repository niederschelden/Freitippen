# README - Freitippen für Motoren mit 12V Steuerung in Behandlungsliegen

## Overview

This repository contains a circuit for enabling the 12V control of electrically operated treatment beds. The circuit has been developed to prevent the function from being activated until a predetermined number of button presses have occurred. This development was initiated in response to the recommendation by the Federal Institute for Drugs and Medical Devices (BfArM) under case number 0785/03. This recommendation was considered binding by the federal states (Bundesländer) and was subsequently referenced in a separate document by BGW (Institution for Statutory Accident Insurance and Prevention in the Health and Welfare Services).

## Background

Originally, the recommendation by BfArM was developed by the Working Committee for Medical Devices (Arbeitsausschuss für Medizinprodukte, AGMP), with the supervisory authority of Hamburg taking the lead. BfArM participated in the session as a guest, providing information on the topic based on known incidents. Other relevant entities, such as the BGW, were also involved in the development. The supervision of medical devices falls within the jurisdiction of the federal states according to the German Basic Law. Despite my inquiry to BfArM, I was unable to identify the responsible parties, leading me to decide on the development of a counter-concept instead.

## Consequence

Due to the unilateral decision, which, to my knowledge, did not involve professional associations or the industry, the replacement of numerous treatment beds or their motors became necessary. Even a well-known German motor manufacturer was unwilling to provide me with a datasheet for their motors. Thus, there was resistance from all sides. Ultimately, the consequence is a significant carbon footprint, a financial burden on practices, and a complex legal situation. Unlike in 2004, no technical solutions for retrofitting were introduced to the market (similar to how lock boxes were used back then).

## Circuit

The objective is to showcase the feasibility of implementing a safety-critical locking mechanism to prevent unintended activation, akin to the operation of loading dock ramps with foot control, a concept familiar to me since the 1980s, as well as contemporary practices seen in newly manufactured motors. The devised circuit facilitates the securement of the 12V control for electrically powered bed motors. By design, the circuit halts the activation sequence of the internal relay until a predefined number of button presses have transpired. The activation signal is ascertained using a voltage divider and a debouncing capacitor, all the while refraining from engaging the motor's relay. Upon successive button presses, a transistor liberates the relay's grounding.

## Note

This repository solely serves to illustrate the circuit and the context in which it was developed. The result is not a medical device and must not be used for applications on beds within the jurisdiction of BGW and BfArM.
