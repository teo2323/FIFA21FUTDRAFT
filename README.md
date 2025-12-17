# ⚽ FIFA 21 FUT Draft Simulator - OOP C++ Project

Un simulator de FIFA Ultimate Team Draft, dezvoltat în **C++** folosind biblioteca grafică **SFML**. Proiectul utilizeaza carduri din FIFA 21, preluate de pe aplicatia futbin
https://www.futbin.com/21/draft-simulator
---

## Descriere

Aplicația simulează modul "Draft" din jocurile mai vechi de FIFA, pana la FIFA 23. Utilizatorul alege o formație, apoi completează fiecare poziție din echipă alegând dintr-o selecție de 5 jucători random. Obiectivul este construirea unei echipe cu un **Rating** mare și o **Chimie (Chemistry)** perfectă (de 100 de puncte).

### ✨ Funcționalități Cheie

* **Sistem de Draft:** Alegerea formației (4-3-3, 4-4-2, 5-3-2, etc.) și popularea echipei prin selecție interactivă.
* **Vizualizare Dinamică:**
    * Jucătorii sunt așezați pe teren în funcție de coordonatele specifice formației.
    * **Link-uri Vizuale:** Linii colorate între jucători (Roșu/Portocaliu/Galben/Verde) care se actualizează în timp real în funcție de relațiile dintre ei.
* **Interacțiune GUI:**
    * **Swap System:** Click pe doi jucători pentru a le schimba locurile (cu validare logică - ex: Portarul nu poate juca în atac).
    * **Preview:** Click dreapta pentru a mări cardul jucătorului.
    * **Manager:** Selectarea managerului pentru bonusuri de chimie.
* **Statistici:** Calcul în timp real pentru Rating, Chemistry și Overall.
* **Chimie Complexă:** Calcularea chimiei individuale și de echipă pe baza naționalității, ligii și clubului (algoritm similar cu cel real).
* ### 🔗 Sistemul de Chimie (Links)
Algoritmul de chimie, implementat în metoda `calcLink`, determină puterea legăturii dintre doi jucători adiacenți printr-un sistem de punctare bazat pe potrivirea atributelor (**Naționalitate**, **Ligă**, **Club**):

* **Perfect Link (3 pct - Linie Verde):** Jucătorii au aceeași Naționalitate, același Club și aceeași Ligă.
* **Strong Link (2 pct - Linie Galbenă):**
    * Jucătorii au aceeași Ligă și aceeași Naționalitate (dar cluburi diferite).
    * SAU jucătorii sunt colegi la același Club (dar au naționalități diferite).
* **Weak Link (1 pct - Linie Portocalie):** Jucătorii împart doar aceeași Ligă SAU doar aceeași Naționalitate.
* **Dead Link (0 pct - Linie Roșie):** Nu există nicio coincidență între atributele celor doi jucători.
* Jucatorii de tip legenda au un comportament special in calculul chimiei, ei "preluand" liga jucatorului adiacent.



---

## 🎮 Controale

| Tastă / Acțiune | Efect |
| :--- | :--- |
| **Click Dreapta** (Ține apăsat) | Face **Preview** (mărește) formatia. 
| **Click Stânga** (Pe formatie) | Selectează formatia. |
| **Click Dreapta** (Ține apăsat, in timpul alegerilor) | Face **Preview** (mărește) cardul jucătorului. |
| **Click Stânga** (Pe jucător, in timpul algerilor) | Selectează jucătorul (si il introduce in echipa). |
| **Click Stânga** (Pe jucător, dupa ce a fost completata echipa) | Selectează jucătorul (se mareste). |
| **Click Stânga** (Pe al 2-lea jucător) | Face **SWAP** între cei doi jucători. |


