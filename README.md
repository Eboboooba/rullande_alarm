# Rullande Alarm System

## Översikt
Detta projekt använder en Arduino för att skapa ett rullande larm med en bil som reagerar på avståndssensorer och en inställd alarmtid. Vid alarmtiden startar bilen och rullar framåt tills den upptäcker ett hinder, vid vilket den svänger bort från hindret. När användaren trycker på stoppknappen, avbryts alarmet och bilen stannar.

## Komponenter
1. **Arduino** - Mikrokontroller som styr alla funktioner.
2. **RTC (Real-Time Clock) - DS3231** - Används för att hålla reda på aktuell tid.
3. **OLED Display - SSD1306** - Visar aktuell tid och alarmstatus.
4. **Ultraljudssensorer** - 3 stycken (HC-SR04) används för att mäta avstånd till hinder.
5. **Motorer** - Styr bilens rörelse framåt och dess svängningar.
6. **Buzzer** - Används för att spela upp larmtonen vid alarmtiden.
7. **Tryckknappar** - Används för att sätta alarmtid, aktivera alarmet och stoppa alarmet.

## Anslutningar
- **Motorer**:
  - Vänsterhjul (motor): Pin 2
  - Högerhjul (motor): Pin 3
- **Avståndssensorer** (ultraljud):
  - Sensor 1: Trigger Pin - 4, Echo Pin - 5
  - Sensor 2: Trigger Pin - 6, Echo Pin - 7
  - Sensor 3: Trigger Pin - 8, Echo Pin - 9
- **Buzzer**: Pin A0
- **Tryckknappar**:
  - Button Set Hour: Pin 10
  - Button Set Minute: Pin 11
  - Button Toggle Alarm: Pin 12
  - Button Stop Alarm: Pin 13

## Funktioner
- **Aktuell Tid**: Visas på OLED-skärmen i formatet `HH:MM:SS`.
- **Alarmtid**: Kan ställas in med knappar för att justera timme och minut. Alarmet aktiveras när den aktuella tiden matchar alarmtiden.
- **Alarm**: När alarmet triggas, startar bilen och kör framåt. Om ett hinder upptäcks på mindre än 20 cm, svänger bilen bort från hindret.
- **Stoppknapp**: När stoppknappen trycks, avbryts alarmet och bilen stannar.
  
## Kommandon och Funktioner
1. **Ställa in Alarm**: 
   - Knapp för att sätta timme: `Button Set Hour`
   - Knapp för att sätta minut: `Button Set Minute`
   - Knapp för att aktivera/deaktivera alarmet: `Button Toggle Alarm`
   - Knapp för att stoppa alarmet: `Button Stop Alarm`

2. **Bilen Rörelse**:
   - **Framåt**: Bilen rör sig framåt tills den når ett hinder.
   - **Sväng**: Om ett hinder är närmare än 20 cm, svänger bilen vänster för att undvika kollision.
   - **Stopp**: När stoppknappen trycks, stannar bilen.

## Installation och Användning
1. Ladda upp koden till din Arduino via Arduino IDE.
2. Koppla alla komponenter enligt de angivna pinnarna.
3. När koden körs:
   - Ställ in alarmtiden genom att trycka på knapparna för timme och minut.
   - Aktivera alarmet genom att trycka på knappen för att växla alarmstatus.
   - När alarmtiden inträffar, kommer bilen att börja röra sig framåt och reagera på hinder genom att svänga bort från dem.
   - Tryck på stoppknappen för att stoppa alarmet och stanna bilen.

## Problem och Felsökning
- **Bilen svänger inte**: Kontrollera att alla avståndssensorer är korrekt anslutna och fungerar. Se till att sensorerna är placerade så att de kan upptäcka hinder korrekt.
- **Buzzern fungerar inte**: Kontrollera att pinnen för buzzern är korrekt ansluten och att ingen annan funktion blockerar buzzerkommandot.

## Licens
Detta projekt är tillgängligt under [MIT License](https://opensource.org/licenses/MIT).
