# README - Freitippen für Motoren  mit 12V Steuerung in Behandlungsliegen

## Übersicht

Dieses Repository enthält eine Schaltung zur Freischaltung der 12V-Steuerung von elektrisch angesteuerten Behandlungsliegen. Die Schaltung wurde entwickelt, um die Funktion zu blockieren, bis eine festgelegte Anzahl von Tastendrücken erfolgt ist. Diese Entwicklung wurde als Reaktion auf die Empfehlung des Bundesministeriums für Arzneimittel und Medizinprodukte (BfArM) Fall-Nr. 0785/03 initiiert. Diese Empfehlung wurde von den Bundesländern als verbindlich angesehen und von der BGW in einem eigenen Dokument erneut aufgegriffen.

## Hintergrund

Ursprünglich wurde die Empfehlung des BfArM vom Arbeitsausschuss für Medizinprodukte (AGMP) erarbeitet, wobei die Landesbehörde Hamburg den Vorsitz und die Federführung innehatte. Das BfArM nahm als Gast an der Sitzung teil und trug Informationen zu dem Thema aufgrund bekannter Vorfälle bei. Weitere relevante Stellen, wie die Berufsgenossenschaft BGW, waren ebenfalls an der Erarbeitung beteiligt. Die Überwachung von Medizinprodukten fällt gemäß dem deutschen Grundgesetz in die Zuständigkeit der Bundesländer. Da es mir trotz einer Anfrage beim BfArM nicht gelungen ist, die verantwortlichen Parteien zu identifizieren, entschied ich mich stattdessen für die Entwicklung eines Gegenkonzepts.

## Schaltung

Das Ziel ist der Nachweis, dass die nachträgliche Implementierung einer sicherheitsrelevanten Sperrfunktion gegen ungewolltes Einschalten möglich gewesen wäre, ähnlich wie sie jetzt bei neu ausgelieferten Motoren verwendet wird. Die entwickelte Schaltung ermöglicht die Sperrung der 12V-Steuerung von elektrisch angesteuerten Liegenmotoren. Die Schaltung blockiert den Einschaltvorgang des internen Relais, bis eine vordefinierte Anzahl von Tastendrücken erfolgt ist. Die Schaltung erfasst den Einschaltwunsch mithilfe eines Spannungsteilers und eines Entprellungskondensators, ohne das Relais des Motors zu aktivieren. Nach mehrfachen Tastendrücken wird die Relaismasse über einen Transistor freigeschaltet.

## Hinweis

Dieses Repository dient lediglich der Darstellung der Schaltung und des Kontexts, in dem sie entwickelt wurde. Das Ergebnis ist kein Medizinprodukt und darf nicht zur Anwendung an Liegen im Einflussbereich der BGW und des BfArM verwendet werden.
