# C Dynamic Projects


Three menu-driven C programs that use structs, dynamic memory (`malloc`/`realloc`/`free`), pointer traversal and function pointers.

| # | Project | File |
|---|---------|------|
| 1 | Dynamic Academic Performance and Risk Analysis Engine | `academic.c` |
| 2 | Dynamic Text Mining and Linguistic Pattern Analyzer | `text_mining.c` |
| 3 | Dynamic IoT Event Monitoring and Callback Engine | `event_monitor.c` |



---

## Project 1: Academic Risk Engine

- Starts with 15 students and room for 20. The array doubles when full and halves when it is under a quarter full (never below 20), so it avoids resizing on every add or remove.
- Duplicate IDs, bad grades and empty names are rejected. All input goes through `check_line`, `check_int` and `check_float`.
- Menu choices are dispatched through an array of function pointers. Sorting uses `qsort` with three comparison functions (grade, name with `strcmp`, ID).
- Average, top student and display walk the array with pointers. The top student is the first one found if grades tie.
- Name search is exact and case-sensitive.

**Custom analysis: Academic Risk Index.** The user picks a threshold.
- Grade at or above the threshold: risk = 0 (Safe).
- Otherwise: `risk = (threshold - grade) / threshold * 100`.
- Levels: Low (up to 25), Moderate (up to 50), High (above 50).

A higher score means a bigger gap to the threshold. The summary report uses the pass mark (50) as its threshold.

## Project 2: Text Mining Analyzer

- Source text: the abstract of *Bitcoin: A Peer-to-Peer Electronic Cash System* by Satoshi Nakamoto (the full PDF is in the repo). It is copied into a `malloc`'d buffer.
- Words are found with a pointer walk and stored in a dynamic `wordStat` array that starts at 8 entries and doubles when full.
- **Word rules:** everything is lowercased; an apostrophe stays inside a word when a letter follows it (`they'll`); hyphens, digits and other punctuation separate words (`proof-of-work` becomes `proof`, `of`, `work`); words longer than 39 letters are cut at 39. With these rules the paragraph has 192 words.
- Search is case-insensitive. Sorting is alphabetical (`strcmp`) or by frequency, using `qsort`.
- The text summary also prints the addresses of the text buffer, the stats array and one entry (`%p`).

**Custom analysis: Lexical Diversity Score** = distinct words / total words. Close to 1 means varied vocabulary, close to 0 means heavy repetition (0.7 and above varied, 0.4 to 0.7 moderate, below 0.4 repetitive).

## Project 3: IoT Callback Engine

- Devices live in a dynamic array that grows by one on register and shrinks on remove. Each reading is stored in a `union` (temperature, pressure or voltage), and the device `type` decides which member is valid.
- Readings are random (`rand()` seeded with `time`):

| Type | Generated range | Safe range |
|------|-----------------|------------|
| Temperature | 10 to 80 C | 15 to 70 C |
| Pressure | 50 to 150 kPa | 70 to 130 kPa |
| Battery | 2.5 to 4.5 V | 3.0 to 4.2 V |

- `process_device(device, callback)` runs the callback chosen by device type from an array of function pointers (`temperature_monitor`, `pressure_monitor`, `battery_monitor`). Every alert shows the device, reading, expected range, reason and responsible callback.

**Custom callback: Sensor Drift Detector.** Flags a reading more than 35% away from the device's previous reading (outside `previous * 0.65` to `previous * 1.35`). It needs at least two readings, and it only runs when the type's own monitor found nothing.