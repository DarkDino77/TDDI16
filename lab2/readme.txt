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
compute_summary behöver räkna ut ett hashvärde som är relativt unikt per bild
utan att behöva lagra allt för mycket information om varje bild. Det behöver 
dessutom vara snabb för att nå upp till de krav om körtid som finns i labben.
Resultatet bör dessutom gå att lagra i en size_t utan att förlora alltför mycket
information då ett hashvärde av den typen krävs för att kunna lagra något i en
unordered_map.
Vi anser att den givna funktionen uppfyller egenskaperna. Vi såg inte att den markerade
alltför olika bilder som dubbletter. tidskomplexitet var inte heller för hög.
Ett problem med den var dock att den bara undersökte skillnaden i ljusstyrka mellan två olika
pixlar vilket gjorde att bilder med väldigt olika ljusstyrkor överlag, men med samma relativa
ljusstyrkeskillnad mellan pixlar, markerades som dubletter vilket kanske inte är önskvärt. 

- Ser du några problem med metoden för att se om två bilder är lika dana?
  Föreslå en alternativ metod för att hitta bilder som är lika. Vad har
  ditt/dina förslag för för- och nackdelar jämfört med det som föreslås i
  labben? Fokusera exempelvis på vilka typer av skillnader i bilder som
  hanteras, eller vilken tidskomplexitet som kan uppnås. Ditt förslag behöver
  inte vara snabbare än det som föreslås i labben, men du ska komma på
  åtminstone en fördel med din metod.
I vår lösning kollar vi bara den relativa ljusstyrkan mellan pixlar. Detta gör att om
två bilder har väldigt skilda ljusstyrkor kommer de ändå räknas som dubletter av varandra
så länge den relativa ljusstyrkan mellan närliggande pixlar är liknande.

Man skulle kunna lagra den genomsnittliga ljusstyrkan i Image_Summary och göra det till en del av
hashen. Detta skulle kunna ske genom att summera ljusstyrkan i alla pixlar och sedan dividera summan
med antalet pixlar. För att överföra detta till hash-värdet skulle man sedan kunna normalisera detta
genomsnitt till ett värde mellan 0 och 7 och lagra detta normaliserade genomsnitt i de översta 4
bitsen i hashvärdet. Man bör då även lägga till en jämförelse mellan ljusstyrkor i den överlagrade jämförelseoperatorn
för Image_Summary.

Skulle man bara ta ljusstyrkan i beaktande under jämförelsen men inte lyckas inkludera det i hashen skulle
det inte påverka tidskomplexiteten men däremot påverka körtiden. Detta eftersom bilder som inte är dubbletter
skulle få samma hashvärde och därför hamna i samma bucket. Dessa kollisioner skulle sakta ner körtiden något
eftersom man skulle behöva hantera kollisionerna vid insättning och lookup.

Man kan också jämnföra andra värden än brigthness som till exempel rgb värden.