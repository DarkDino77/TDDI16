Mönsterigenkänning
==================

- Ungefärligt antal timmar spenderade på labben (valfritt):
  5 timmar

- Vad är tidskomplexiteten (i ordo-notation) för värstafallstiden av
  programmen som en funktion av N (antal punkter). Ge en kort motivering.

  Matchar brute-lösningen sitt värstafall i praktiken, eller har den ett
  medelfall som är bättre?

brute:
  O(N³) i medelfall  (i värsta fallet är den O(N⁴) men medelfallet sker tillräckligt ofta).
  Medelfallet kommer från att algoritmen undersöker varje kombination av tre punkter från punktmängden.
  Om tre sådana punkter hittas gås hela mängden av punkter igenom igen, därav O(N⁴) i värsta fall.
fast:
  O(N²logN). Detta beror på att listan gås igenom i linjär tid i den yttre loopen. För varje punkt i listan
  sorteras den dessutom i O(NlogN) tid. För varje sådan sortering gås sedan listan igenom på nytt i
  linjär tid, vilket totalt sett resulterar i O(N²logN).

- Fyll i tabellen nedan med riktiga körtider i sekunder när det känns
  vettigt att vänta på hela beräkningen.
  Ge uppskattningar av körtiden (baserat på tidskomplexiteten)
  i övriga fall.
    
      N     brute(s)      sortering(s)
 ----------------------------------
    150     0.085         0.009
    200     0.165         0.019
    300     0.545         0.051
    400     1.276         0.090
    800     10.020        0.338
   1600     81.950        1.429
   3200     651.543       6.020
   6400     ~2606.172     26.108
  12800     ~10424.688    113.260


- Energianvändning

  Antag att du använder mönsterigenkänningsprogrammet för att analysera
  data från en kamera. Kameran sitter i en byggnad och tar en bild
  på stommen av byggnaden var 30:e minut. Bilden förbehandlas sedan
  lite, innan punkter som representerar stommen skickas till
  mönsterigenkänningsprogrammet. Hittas inte tillräckligt många raka
  linjer så betyder det att något håller på att gå sönder, och
  att byggnaden behöver noggrannare inspektion.

  Hur mycket energi sparar du på ett år om du använder din snabbare
  sorteringslösning i stället för brute-lösningen? Du kan anta följande:
  - Systemet körs 24/7 under hela året.
  - Inget annat körs på det här systemet.
  - Systemet drar 8 W när det inte gör något (idle)
  - Systemet drar 36 W när det arbetar (med 1 kärna)
  - Räkna med att ditt program körs var 30:e minut (= 2 gånger/timme)
  - För- och efterbehandling är snabba, så vi kan bortse från dem
  - Indata till programmet innehåller ca 6400 punkter
  - Det är inte skottår (= 365 dagar)

  Att jämföra med drar en kombinerad kyl/frys ca 200 kWh per år
  (enligt Energimyndigheten).
 
  Kom ihåg: energi mäts ofta i kWh, vilket är:
   energi (kWh) = effekt (kW) * tid (h)

  Tips: ett sätt att räkna på är att först räkna förbrukningen av
  ett system som inte gör något på ett helt år, sedan lägga till
  den extra förbrukningen (36 W - 8 W = 28 W) för tiden som systemet
  är aktiv.

  (Siffrorna är löst baserade på en Intel i9-9900K, vi räknar bara på
  CPU:n för enkelhets skull, besparingarna blir sannolikt större om
  vi räknar på större delar av systemet, även om andra komponenter
  också drar ström i "idle".)


Förbrukning av brute på ett år: 519.72 kWh

Förbrukning av sotering på ett år:  83.30 kWh

Skillnad:  436.42 kWh


