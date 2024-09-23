Bildmatchning
=============

- Ungefärligt antal timmar spenderade på labben (valfritt):

4 timmar


- Vad är tidskomplexiteten på "slow.cpp" och din implementation av "fast.cpp",
  uttryckt i antalet bilder (n).

slow: O(n²)
fast: O(n)


- Hur lång tid tar det att köra "slow.cpp" respektive "fast.cpp" på de olika
  datamängderna?
  Tips: Använd flaggan "--nowindow" för enklare tidsmätning.
  Tips: Det är okej att uppskatta tidsåtgången för de fall du inte orkar vänta
  på att de blir klara.
  Tips: Vid uppskattning av körtid för "slow.cpp" är det en bra idé att beräkna
  tiden det tar att läsa in (och skala ner) bilderna separat från tiden det tar att
  jämföra bilderna. (Varför?)

|--------+-----------+----------+----------|
|        | inläsning | slow.cpp | fast.cpp |
|--------+-----------+----------+----------|
| tiny   |   ~95ms   |  ~150ms  | ~124ms   |
| small  |  ~590ms   |  ~728ms  | ~710ms   |
| medium |  ~2550ms  |  ~3440ms | ~2770ms  |
| large  |  ~65000ms | ~505592ms| ~64000ms |
|--------+-----------+----------+----------|


- Testa olika värden på "summary_size" (exempelvis mellan 6 och 10). Hur
  påverkar detta vilka dubbletter som hittas i datamängden "large"?

mindre summary_size ger fler dubbletter eftersom ju högre summary_size desto mer
exakta jämförelser görs.

- Algoritmen som implementeras i "compute_summary" kan ses som att vi beräknar
  en hash av en bild. Det är dock inte helt lätt att hitta en bra sådan funktion
  som helt motsvarar vad vi egentligen är ute efter. Vilken eller vilka
  egenskaper behöver "compute_summary" ha för att vi ska kunna lösa problemet i
  labben? Tycker du att den givna funktionen uppfyller dessa egenskaper?

Funktionen behöver hitta bilder som är relativt lika varandra. Den ska inte räkna
bilder som är alltför olika varandra som dubbletter men inte heller kräva att de
är nästan identiska för att räkna dem som dubbletter.

- Ser du några problem med metoden för att se om två bilder är lika dana?
  Föreslå en alternativ metod för att hitta bilder som är lika. Vad har
  ditt/dina förslag för för- och nackdelar jämfört med det som föreslås i
  labben? Fokusera exempelvis på vilka typer av skillnader i bilder som
  hanteras, eller vilken tidskomplexitet som kan uppnås. Ditt förslag behöver
  inte vara snabbare än det som föreslås i labben, men du ska komma på
  åtminstone en fördel med din metod.
I vår lösning kollar vi bara den relativa ljusstyrkan mellan pixlar. Detta gör att om
två bilder har väldigt skilda ljusstyrkor kommer de ändå räknas som dubletter av varandra.
Man skulle istället kunna ha i åtanke den absoluta skillnaden i ljusstyrka mellan bilderna
ifall man tycker att detta är ett problem.

Man skulle kunna lagra den genomsnittliga ljusstyrkan i Image_Summary och göra det till en del av
hashen, t.ex skulle ljusstyrkan kunna normaliseras till ett värde mellan 0 och 8 och använda de översta
2 bitsen i hashvärdet för att lagra detta genomsnitt. På så sätt får inte två bilder med väldigt skilda ljusstyrkor
samma hashvärde. Man bör då även lägga till en jämförelse mellan ljusstyrkor i den överlagrade jämförelseoperatorn
för Image_Summary.

Man kan också jämnföra andra värden en brigthness som till exemple rgb värden