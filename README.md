# HUMAN BENCHMARK

## Athena

<div align="center">

[Platzhalter Logo]

### [Projektbeiträge: Eigener Beitrag](https://gitlab.iue.fh-kiel.de/aem/aem-2024/aem-team-1/human-benchmark/aem_projekt_beitraege/-/tree/main/Projektbeitrag?ref_type=heads) | [Projektdokumentation im Wiki](https://gitlab.iue.fh-kiel.de/aem/aem-2024/aem-team-1/human-benchmark/athena/-/wikis/home)
</div>

## Produktvision
Das **Ziel** des Projektes ist es, eine unterhaltsame und benutzerfreundliche Plattform für die kognitive Leistungsmessung und -verbesserung durch Tests, im weiteren Spiele genannt, zu erstellen. Diese Spiele werden im Speziellen auf Fähigkeiten, wie Reaktionsschnelligkeit, Erinnerungsvermögen, Mustererkennung, Resilienz sowie Moral abgestimmt sein. Dadurch soll Anwärter:innen auf Assessment-Centern bei der Polizei und der Feuerwehr eine erste Abschätzung gegeben werden, ob sie sich für diese Art von Arbeit eignen und sich gegebenenfalls verbessern. (Wörtlich aus der Produktwahl übernommen.)

Die **Zielgruppe** von Human Benchmark umfasst potenzielle Teilnehmer:innen an Assessment-Centern von Feuerwehr und Polizei.

Ihr **Bedarf** besteht darin, sich effektiv auf anspruchsvolle und selektive Tests vorzubereiten, wobei die Möglichkeit besteht, sich mit anderen zu vergleichen. Zusätzlich bietet die Anwendung eine Grundlage zur Bewertung der Eignung für das Berufsfeld, indem sie mentale und kognitive Fähigkeiten trainiert.

Aufgrund des **Wettbewerbscharakters** von Assessment-Centern sind die Nutzer:innen bereit, finanzielle Ressourcen für eine optimale Vorbereitung einzusetzen. Diese Einschätzung wird durch die Vielzahl von Büchern und Anwendungen zum Thema Assessment-Center gestützt.

Der **Mehrwert** unserer Anwendung besteht darin, das Konzept der Leistungsmessung spielerisch zu vermitteln, ähnlich wie es in Assessment-Centern praktiziert wird.

## Unser Team
Gruppe: AEM-1
 
| Rolle | Teilnehmer              | Matr.-Nr. |
| ----- |-------------------------|-----------|
| PO | Jane Schönfeld          | 941120    |
| SM | Benjamin Puhani         | 941077    |
| DEV | Jannik Brozy            | 940986    |
| DEV | René Marcel Tinter      | 940995    |
| DEV | Caspar Quast            | 941155    |
| DEV | Noah Zepner             | 941167    |
| DEV | Timo Michaelis          | 941362    |
| DEV | Sebastian Elisa Reineke | 941070    |
| DEV | Karlina Jung            | 941169    |
| DEV | Jonas Neumann           | 941341    |
| DEV | Gerrit Koch             | 941213    |


## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Projektstruktur](#projektstruktur)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Dependencies für Linux](#dependencies-für-linux)
  - [Building the Project](#building-the-project)
  - [Terminal Installation](#terminal-installation)

## Features

- Kognitive Leistungsmessung und -verbesserung durch Tests
- Spiele in den Kategorien 
  - Reaktionsschnelligkeit
  - Erinnerungsvermögen
  - Mustererkennung
  - Resilienz
  - Moral
- Detaillierte Statistiken und Vergleiche
- Wettbewerbscharakter
- Vorbereitung auf Assessment-Center für Polizei und Feuerwehr

## Getting Started

### Projektstruktur

Das Projekt ist in folgende Verzeichnisse unterteilt:
- `lib`: Enthält die Bibliotheken, die für das Projekt benötigt werden.
- `src`: Enthält den Quellcode des Projekts.
  - `App`: Enthält die Implementierung der Anwendung.
  - `main`: Ist der Einstiegspunkt des Projekts. Startet die App.

### Prerequisites

Zum Arbeiten mit dem Projekt ist es empfohlen die Entwicklungsumgebung [CLion](https://www.jetbrains.com/de-de/clion/) zu verwenden. Die folgende Anleitung konzentriert sich auf die Verwendung von CLion.

Alternativ kann auch [Visual Studio Code](https://code.visualstudio.com/) in Verbindung mit der [C/C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) verwendet werden. Hiefür müssen noch [CMake](https://cmake.org/) und [MinGW](https://sourceforge.net/projects/mingw-w64/) installiert werden.

### Installation

1. Sollte ein Projekt in CLion offen sein, dieses mit `File > Close Project` schließen.
2. Über `Get from VCS` das Projekt von der Git-URL `https://gitlab.iue.fh-kiel.de/aem/aem-2024/aem-team-1/human-benchmark/athena.git` klonen.
3. Zustimmen, dass dem Projekt vertraut wird und anschließend sollte das Projekt in CLion geöffnet sein.
4. Die erforderlichen Bibliotheken werden per CMake automatisch heruntergeladen und installiert.

### Dependencies für Linux
Folgende dependencies müssen installiert werden, wenn auf Linux entwickelt werden will.
```bash
sudo apt -y install libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libxinerama-dev libxi-dev clang-tidy zip
```

### Building the Project

1. In CLion oben rechts die Konfiguration auf `Athena` setzen.
2. Das grüne Play-Symbol in der IDE klicken.
3. Das Projekt sollte nun gebaut und ausgeführt werden.

### Terminal Installation

IDE unabhängige Installation für: Windows, Linux, Mac

Vorausgesetzte Programme: 
Cmake, Git, C++ Compiler (MingW, Visual Studio Compiler, G++)

```bash
git clone git@gitlab.iue.fh-kiel.de:aem/aem-2024/aem-team-1/human-benchmark/athena.git
cd athena
mkdir build && cd build
cmake ..
cmake --build .
```