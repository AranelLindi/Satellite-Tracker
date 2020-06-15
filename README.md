# Satellite-Tracker
Berechnet Überflugzeitpunkte sowie Koordinaten und Bahneigenschaften von Satelliten auf Basis des SGP4 Models und mit TLE (Two Line Element) als Grundlage.

## **calc/** sat_calculation.h/cpp ##
Stellt Funktionen bereit um verschiedene Bahneigenschaften eines Satelliten zu einem gegebenen Zeitpunkt t zur Berechnen und nutzt dabei den julianischen Kalendar sowie das ECI System.

## **calendar/** GregorianCalendar.h/cpp ##
Stellt einen Container für ein gregorianisches Datum inklusive Uhrzeit zur Verfügung sowie verschiedene Operatorüberladungen die dieses Objekt betreffen.

## **contact/** contact_time.h/cpp ##
Berechnet Kontaktzeitpunkte/-verluste eines Satelliten über einen gegebenen Zeitraum und printet die Ergebnisse in eine Textdatei.

----------------------------------------------------------

### **main_calculation.cpp**###
Beispielhafte Implementierung der Berechnung von sat_calculation.

### **main_contact.cpp**###
Beispielhafte Implementierung der Bestimmung von Kontaktzeiträumen via contact_time.

### **main_position.cpp**###
Berechnet beispielhaft die Koordinaten der Flugbahn eines Satelliten.