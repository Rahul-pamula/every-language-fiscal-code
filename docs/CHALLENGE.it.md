# Every Language Challenge
## Un algoritmo, tutti i linguaggi del mondo
### Progetto open source promosso da Openapi — Specifica e piano operativo

> **Aggiornamento, ottobre 2026.** I traguardi ora sono 64 (2⁶), 99, 128, 256
> e oltre, senza scadenze. Vedi [ROADMAP.md](../ROADMAP.md). Gli obiettivi di
> 100 linguaggi riportati sotto fanno parte della proposta originale.

**Stato:** proposta progettuale consolidata  
**Data:** 14 ottobre 2026  
**Organizzazione promotrice:** Openapi  
**Prima implementazione:** Italian Fiscal Code  
**Obiettivo iniziale:** 100 linguaggi  
**Obiettivo esteso:** 256+ linguaggi  
**Ambizione:** tentare un Guinness World Record verificabile

---

# 1. Visione

Realizzare una collezione open source di implementazioni indipendenti dello stesso algoritmo in quanti più linguaggi di programmazione possibile.

L'obiettivo non è semplicemente pubblicare tanti frammenti di codice, ma dimostrare che un unico problema computazionale può essere risolto, eseguito e verificato attraverso linguaggi appartenenti a epoche, paradigmi e ambienti profondamente diversi.

Dal COBOL al Rust, dal FORTRAN al Python, dal Prolog all'Assembly, ogni implementazione dovrà rispettare una specifica comune e superare i medesimi test.

Il progetto nasce per coinvolgere la comunità internazionale degli sviluppatori, valorizzare linguaggi poco utilizzati o storici e costruire una raccolta permanente di codice realmente funzionante.

Il progetto dovrà essere utile e interessante anche qualora non ottenesse alcun riconoscimento Guinness.

**Principio fondamentale: un algoritmo, molti linguaggi, una sola definizione di correttezza.**

# 2. Origine delle idee

## Prima idea — Giugno 2026

La proposta originale consisteva nello scrivere l'algoritmo del codice fiscale italiano in 100 linguaggi di programmazione, eventualmente estendendo la sfida a tutti i linguaggi per i quali fosse possibile costruire un'implementazione.

Il nome inizialmente proposto era:

**The Fiscal Code Language Challenge**

Successivamente era emersa una struttura più generale:

**Every Language**

con un primo repository denominato:

`every-language-fiscal-code`

La scelta era intenzionale: iniziare dal codice fiscale, ma creare un formato che in futuro potesse accogliere altri algoritmi.

Slogan discussi:

- "Implementing real-world algorithms in every programming language."
- "A community effort to implement the same algorithm everywhere."

Il progetto avrebbe dovuto essere presentato come un'iniziativa collaborativa e non come una semplice campagna pubblicitaria.

## Seconda idea — Ottobre 2026

L'obiettivo si è esteso verso un possibile Guinness World Record, con un'iniziativa promossa da Openapi per generare notorietà, attenzione internazionale e partecipazione degli sviluppatori.

La nuova proposta è stata chiamata:

**100 Languages Challenge**

Un singolo algoritmo, implementato indipendentemente in 100 o più linguaggi, con dimostrazione pubblica della correttezza di ogni implementazione.

È stata considerata anche l'idea di utilizzare SHA-256, un algoritmo standardizzato, sufficientemente complesso e con test vector pubblici.

La preferenza progettuale, però, resta partire dal codice fiscale italiano, mantenendo SHA-256 come possibile seconda sfida o alternativa più universale per il tentativo di record.

# 3. Naming e identità

Si propone di distinguere il nome del progetto generale da quello della sua prima challenge.

**Ecosistema:** Every Language  
**Organizzazione GitHub ipotizzata:** `every-language`  
**Prima repository:** `every-language-fiscal-code`  
**Nome della challenge:** The Fiscal Code Language Challenge

Titolo pubblico:

**One Algorithm. Every Programming Language.**

Sottotitolo:

"An open-source challenge to implement the Italian Fiscal Code algorithm in every programming language."

Openapi figura come promotore e sostenitore dell'iniziativa, senza trasformarla in una demo commerciale delle proprie API.

Il nome Guinness non deve comparire nel nome ufficiale del progetto e non deve essere utilizzato in modo da suggerire una certificazione non ancora ottenuta.

# 4. Perché il codice fiscale

Il codice fiscale italiano rappresenta un problema interessante perché combina diverse tipologie di elaborazione.

Comprende:

- trasformazioni di stringhe;
- estrazione di consonanti e vocali;
- regole di selezione dei caratteri;
- trasformazione di date;
- codifica del sesso e del giorno di nascita;
- lookup di codici territoriali;
- aritmetica modulare;
- calcolo di un carattere di controllo;
- gestione dell'omocodia.

È sufficientemente complesso da distinguere una vera implementazione da una semplice dimostrazione del linguaggio.

È anche abbastanza contenuto da poter essere implementato in linguaggi molto diversi, compresi quelli storici e con risorse limitate.

Inoltre, si tratta di un algoritmo reale, con un'applicazione concreta nel mondo amministrativo.

Un punto essenziale è distinguere il calcolo teorico di un codice fiscale dalla sua effettiva attribuzione e validità presso l'Anagrafe tributaria. Il progetto non dovrà dichiarare che un codice generato sia necessariamente quello ufficialmente attribuito a una persona.

# 5. Specifica funzionale della prima challenge

La prima versione deve occuparsi del codice fiscale ordinario a 16 caratteri per persone fisiche.

Input canonico:

```json
{
  "surname": "ROSSI",
  "name": "MARIO",
  "birth_date": "1980-01-01",
  "sex": "M",
  "birthplace_code": "H501"
}
```

Output canonico proposto:

```json
{
  "fiscal_code": "RSSMRA80A01H501U"
}
```

L'esempio sopra è un vettore illustrativo da verificare e includere nella suite ufficiale solo dopo il controllo con un'implementazione di riferimento affidabile.

## Comportamento richiesto

Ogni implementazione deve:

1. Accettare i medesimi dati logici.
2. Applicare la stessa normalizzazione testuale definita dalla specifica.
3. Applicare le regole di codifica del cognome.
4. Applicare le regole di codifica del nome.
5. Codificare anno, mese, giorno e sesso.
6. Utilizzare il codice catastale del comune o dello Stato estero di nascita fornito in input.
7. Calcolare il carattere di controllo.
8. Restituire il codice fiscale risultante.
9. Gestire gli input non validi secondo una convenzione comune.
10. Superare la stessa suite di conformità.

## Estensioni successive

La seconda fase può includere:

- verifica formale di un codice fiscale;
- gestione delle varianti omocodiche;
- decodifica delle informazioni contenute nel codice;
- lookup da comune e Stato estero al codice territoriale;
- gestione delle denominazioni storiche dei luoghi;
- normalizzazione dei nomi con accenti e caratteri non ASCII.

La gestione dell'omocodia dovrà distinguere fra generazione di varianti teoricamente ammissibili e codice effettivamente assegnato dall'autorità.

Il perimetro della prima versione deve rimanere limitato e rigoroso per rendere realizzabile la sfida anche nei linguaggi meno moderni.

# 6. Linguaggi di programmazione

Il primo traguardo è raggiungere almeno 100 implementazioni valide.

La repository dovrà però essere progettata per scalare verso 256 e oltre, senza limiti artificiali.

Famiglie di linguaggi da coinvolgere:

| Famiglia | Esempi |
|---|---|
| General purpose | C, C++, Rust, Go, Zig, Nim |
| Scripting | Python, PHP, Ruby, Perl, Lua, Tcl |
| JVM | Java, Kotlin, Scala, Groovy |
| .NET | C#, F#, VB.NET |
| Funzionali | Haskell, OCaml, Erlang, Elixir |
| Logici | Prolog, Mercury |
| Storici | COBOL, FORTRAN, BASIC, Pascal, Ada |
| Lisp | Common Lisp, Scheme, Clojure, Racket |
| Scientifici | Julia, R, MATLAB, Octave |
| Di sistema | Assembly x86-64, ARM Assembly |
| Altri | Smalltalk, Forth, APL, J, D |

L'elenco è indicativo e non costituisce il conteggio del record.

## Cosa consideriamo un linguaggio distinto?

È necessario stabilire criteri pubblici.

Proposta:

- linguaggio con sintassi e semantica identificabili;
- implementazione scritta effettivamente nel linguaggio dichiarato;
- esecuzione tramite un interprete, compilatore o runtime appropriato;
- comportamento verificabile;
- nessuna semplice invocazione di un'implementazione realizzata in un altro linguaggio.

I dialetti, le versioni e i linguaggi incorporati vanno classificati separatamente.

Per esempio, C11 e C17 non dovrebbero contare automaticamente come due linguaggi distinti.

Anche JavaScript e TypeScript devono essere valutati secondo una regola documentata, senza assumere automaticamente che una differenza di sintassi costituisca un linguaggio aggiuntivo ai fini del record.

La classificazione dovrà essere accettabile anche per un revisore esterno.

# 7. Repository e organizzazione

Struttura GitHub proposta:

```text
every-language-fiscal-code/
├── README.md
├── LICENSE
├── CONTRIBUTING.md
├── CODE_OF_CONDUCT.md
├── SPECIFICATION.md
├── LANGUAGES.md
├── RECORD.md
├── specification/
│   ├── input.schema.json
│   ├── output.schema.json
│   ├── algorithm.md
│   ├── normalization.md
│   └── errors.md
├── test-vectors/
│   ├── basic.json
│   ├── edge-cases.json
│   ├── invalid-inputs.json
│   └── omocodia.json
├── languages/
│   ├── c/
│   ├── cpp/
│   ├── python/
│   ├── php/
│   ├── rust/
│   ├── go/
│   ├── cobol/
│   ├── fortran/
│   ├── prolog/
│   └── ...
├── tools/
│   ├── runner/
│   ├── validator/
│   └── reporter/
├── reports/
├── docs/
└── .github/
    ├── workflows/
    └── ISSUE_TEMPLATE/
```

Ogni implementazione avrà una propria directory.

Esempio:

```text
languages/rust/
├── manifest.yaml
├── README.md
├── src/
│   └── main.rs
└── run.sh
```

Manifest ipotetico:

```yaml
language: Rust
version: "1.x"
implementation: native
source: src/main.rs
run: ./run.sh
tests: shared
status: verified
```

Il campo `status` viene determinato dal sistema di verifica, non dichiarato autonomamente dal contributore.

Il linguaggio Rust è solo un esempio; il manifest non obbliga a utilizzare un determinato sistema di build.

# 8. Interfaccia comune

Ogni implementazione deve essere utilizzabile da un processo esterno.

Si propone una CLI con:

- input JSON su stdin;
- output JSON su stdout;
- diagnostica su stderr;
- exit code coerente;
- nessuna interazione manuale.

Esempio:

```bash
echo '{
  "surname":"ROSSI",
  "name":"MARIO",
  "birth_date":"1980-01-01",
  "sex":"M",
  "birthplace_code":"H501"
}' | ./run.sh
```

Per i linguaggi storici o con difficoltà di parsing JSON, è ammissibile un adattatore esterno limitato al trasporto dei dati.

In questi casi, però, l'adattatore non potrà implementare la logica di calcolo del codice fiscale.

Il manifest dovrà dichiarare esplicitamente la presenza dell'adattatore.

Il cuore computazionale dovrà rimanere nel linguaggio oggetto della challenge.

# 9. Indipendenza delle implementazioni

Questo è uno dei requisiti più importanti.

Ogni versione deve essere una vera implementazione dell'algoritmo, non una chiamata a un servizio esterno o a una libreria che esegue già l'intero calcolo.

Non sono valide come implementazioni indipendenti:

- una funzione che richiama un'API remota per ottenere il codice fiscale;
- un wrapper attorno all'eseguibile di un altro linguaggio;
- un programma che legge semplicemente un risultato precalcolato;
- codice che contiene soltanto casi speciali per superare i test pubblici.

Sono invece ammissibili le funzionalità standard del linguaggio, come manipolazione di stringhe, array, operazioni matematiche e lettura dell'input.

Non bisogna confondere indipendenza dell'algoritmo con divieto assoluto di qualunque dipendenza tecnica.

La regola deve essere sufficientemente rigorosa ma praticabile.

# 10. Sistema centralizzato di verifica

Un runner comune permette di eseguire tutte le implementazioni e confrontarne il risultato.

Workflow:

```text
Contributor
    |
    v
Pull Request
    |
    v
Controllo struttura
    |
    v
Preparazione runtime
    |
    v
Build / interpretazione
    |
    v
Esecuzione test comuni
    |
    v
Confronto risultati
    |
    v
Report verificabile
    |
    v
Merge e classificazione
```

Ogni linguaggio dovrà superare il 100% dei test obbligatori.

Il runner registra:

- linguaggio;
- versione dell'ambiente;
- commit Git;
- hash dei sorgenti;
- sistema operativo e architettura;
- comando eseguito;
- numero di test;
- test riusciti e falliti;
- exit code;
- esito complessivo.

Un linguaggio non diventa automaticamente valido solo perché il contributore ha aperto una Pull Request.

## Stati delle implementazioni

`proposed` — linguaggio richiesto o annunciato.

`in_progress` — implementazione avviata.

`submitted` — implementazione presentata.

`verified` — implementazione che supera tutti i controlli obbligatori.

`rejected` — implementazione non conforme.

`unsupported` — implementazione attualmente non eseguibile nell'infrastruttura disponibile.

Soltanto lo stato `verified` contribuisce al conteggio pubblico.

Il numero destinato all'eventuale record Guinness dovrà inoltre rispettare i criteri ufficiali che saranno comunicati per quello specifico tentativo.

# 11. Test e correttezza

La suite di test è unica per tutti i linguaggi.

Deve includere:

- nomi e cognomi ordinari;
- nomi corti;
- cognomi corti;
- cognomi senza consonanti;
- nomi con almeno quattro consonanti;
- mesi differenti;
- date limite;
- giorni di nascita e sesso;
- codici catastali validi e non validi;
- caratteri di controllo;
- input malformati;
- casi di normalizzazione definiti dalla specifica.

I test devono essere indipendenti dalle singole implementazioni.

La prima implementazione di riferimento potrà essere realizzata in Python o PHP, ma non dovrà costituire l'unica fonte di verità.

Occorrono casi di riferimento controllati separatamente, possibilmente con due implementazioni indipendenti.

Si raccomanda inoltre una suite pubblica e una suite aggiuntiva di verifica, in modo da ridurre il rischio di implementazioni costruite per riconoscere solamente gli esempi conosciuti.

Non devono essere utilizzati dati anagrafici personali reali senza una valida ragione e un'adeguata base giuridica. I vettori sintetici sono preferibili.

# 12. Strategia Guinness World Records

Il progetto nasce con l'ambizione di ottenere il riconoscimento per il maggior numero di linguaggi di programmazione impiegati per implementare e verificare indipendentemente il medesimo algoritmo.

Possibile titolo da proporre:

**Most programming languages used to independently implement and execute the same algorithm**

Questo è un titolo proposto dal progetto, non un record Guinness già riconosciuto.

## Un problema fondamentale

Non basta raccogliere 100 cartelle GitHub per poter dichiarare un record mondiale.

Esistono progetti come Rosetta Code, che raccolgono implementazioni di problemi comuni in numerosi linguaggi.

Il fattore distintivo dovrà quindi essere molto più preciso.

La proposta consiste nel richiedere:

- stesso algoritmo formalmente specificato;
- implementazioni indipendenti;
- comportamento identico;
- esecuzione dimostrabile;
- suite di test comune;
- compilazione o interpretazione verificata;
- manifest per ogni linguaggio;
- report pubblico riproducibile;
- regole di classificazione dei linguaggi;
- prove conservate per una verifica indipendente.

## Misura proposta

La grandezza da misurare è:

**Numero di linguaggi distinti nei quali il medesimo algoritmo è stato implementato ed eseguito con successo secondo criteri condivisi.**

Non è il numero di file, né di righe di codice, né di commit, né di contributor.

## Verifica preliminare

Prima di presentare il progetto come tentativo ufficiale, occorre:

1. Cercare eventuali categorie Guinness già esistenti.
2. Verificare se il titolo proposto è ammissibile.
3. Presentare una domanda per un nuovo titolo, se necessario.
4. Ottenere le linee guida applicabili.
5. Adattare la raccolta delle prove alle linee guida ufficiali.
6. Pianificare il tentativo formale.

Non si deve presumere che la categoria sia approvata.

Secondo i criteri pubblicati da Guinness World Records, un record deve essere misurabile, superabile, standardizzabile e verificabile.

Il carattere italiano del codice fiscale potrebbe rappresentare una difficoltà nella valutazione della standardizzabilità. Per questo il titolo proposto deve riguardare la diversità dei linguaggi che eseguono uno stesso algoritmo, non specificamente il codice fiscale italiano.

Anche questa impostazione richiederà approvazione.

La dimostrazione tecnica pubblica del progetto e il tentativo ufficiale possono essere processi distinti.

È importante richiedere le regole ufficiali prima di eseguire il tentativo formale, non semplicemente prima di terminare la repository.

# 13. Conservazione delle prove

L'infrastruttura dovrà produrre prove riproducibili.

Per ogni versione verificata:

```text
evidence/
  rust/
    manifest.json
    source.sha256
    environment.json
    tests.json
    execution.log
```

Il report finale dovrà contenere:

- identificazione della versione della repository;
- commit e tag Git;
- elenco delle implementazioni valide;
- esito di tutti i test;
- informazioni sugli strumenti di esecuzione;
- checksum degli artefatti;
- data del tentativo;
- eventuali attestazioni dei verificatori.

Le registrazioni video, le dichiarazioni dei testimoni e gli altri documenti richiesti andranno raccolti in conformità alle istruzioni ufficiali, una volta ricevute.

Un workflow GitHub Actions costituisce una prova tecnica utile, ma non garantisce di per sé l'ammissibilità per Guinness.

# 14. Coinvolgimento della community

Il progetto deve essere pensato per crescere grazie a contributori esterni.

L'unità elementare di collaborazione è:

**Un linguaggio ancora non coperto.**

Ogni contributore può scegliere un linguaggio, implementare l'algoritmo e aprire una Pull Request.

Il contributo dovrebbe poter essere realizzato senza necessità di conoscere i sistemi di Openapi o possedere chiavi API.

## Issue GitHub

La repository potrà generare una issue per ogni linguaggio non ancora disponibile.

Esempi:

- Implement Italian Fiscal Code in COBOL
- Implement Italian Fiscal Code in Prolog
- Implement Italian Fiscal Code in FORTRAN
- Implement Italian Fiscal Code in Ada

Le issue più semplici potranno utilizzare `good first issue`, quando questo descriva realmente la difficoltà.

Altre etichette:

`help wanted`, `language:rust`, `language:cobol`, `historical-language`, `needs-runtime`, `tests-failing`.

La distribuzione delle issue potrà appoggiarsi anche ai portali per contributor open source già valutati per gli altri repository Openapi, incluso Up For Grabs.

## Riconoscimento dei contributor

Ogni implementazione includerà:

- nome o handle dell'autore, se desiderato;
- riferimento alla Pull Request;
- data di integrazione;
- linguaggio;
- stato di verifica.

Una pagina pubblica potrà mostrare la comunità che ha contribuito al progetto, senza trasformare il contributo in una competizione individuale obbligatoria.

# 15. Leaderboard e dashboard

La pagina principale dovrà rendere immediatamente comprensibile il progresso.

Esempio:

```text
EVERY LANGUAGE CHALLENGE

One Algorithm. Every Language.

Verified implementations: 42 / 100

Languages proposed:      120
Languages in progress:    18
Contributors:             35
Test suite:               v1.0

Next milestone:           50 languages
```

Questi numeri sono puramente illustrativi.

La dashboard dovrà mostrare:

- progresso rispetto a 100;
- progresso verso 256;
- elenco completo dei linguaggi;
- stato di ogni implementazione;
- autore o autori;
- risultati dei test;
- link al sorgente;
- informazioni per contribuire.

È preferibile una pagina statica generata dai manifest e dai report della CI, senza bisogno di database, backend o infrastruttura complessa.

# 16. Roadmap

## Fase 0 — Definizione e verifica di fattibilità

- Formalizzare l'algoritmo del codice fiscale.
- Definire il perimetro della prima versione.
- Individuare fonti normative e dati di riferimento.
- Stabilire le regole per distinguere i linguaggi.
- Ricercare precedenti e categorie Guinness.
- Preparare una proposta di titolo per Guinness.

Questa fase determina i requisiti fondamentali.

## Fase 1 — Prototipo

Realizzare:

- struttura della repository;
- specifica tecnica;
- vettori di test;
- implementazione di riferimento;
- runner;
- prime cinque implementazioni.

L'obiettivo è dimostrare che linguaggi diversi possono condividere gli stessi test.

## Fase 2 — Validazione dell'architettura

Portare il progetto a circa 10 linguaggi, scegliendo famiglie molto diverse.

Set consigliato:

C, Python, PHP, JavaScript, Rust, Java, COBOL, FORTRAN, Prolog e Haskell.

Questa fase deve verificare soprattutto i casi difficili: toolchain, parsing, runtime, compatibilità e normalizzazione degli output.

## Fase 3 — Lancio pubblico

Pubblicare:

- README in inglese;
- documentazione per contributor;
- pagina del progresso;
- primi risultati della CI;
- issue per nuovi linguaggi;
- guida per aggiungere un'implementazione.

Promuovere il progetto sulle comunità dedicate a open source, programmazione, linguaggi storici e computer science.

Il messaggio principale non deve essere "guardate la nostra azienda", ma "aiutateci a eseguire lo stesso algoritmo in ogni linguaggio".

## Fase 4 — Milestone 25, 50 e 100

La crescita verrà misurata tramite implementazioni verificate.

Ogni traguardo potrà diventare un'occasione per pubblicare un resoconto tecnico:

- linguaggi più insoliti;
- problemi emersi;
- differenze fra paradigmi;
- strumenti di compilazione recuperati;
- contributi della comunità.

## Fase 5 — Tentativo di record

Solo dopo l'approvazione della categoria e la ricezione delle regole ufficiali si potrà definire con precisione il tentativo Guinness.

L'infrastruttura di esecuzione e raccolta delle prove dovrà essere adattata alle condizioni richieste.

## Fase 6 — Espansione

Dopo il traguardo dei 100 linguaggi, il progetto continuerà verso 256 e oltre.

Il raggiungimento di 100 non dovrà essere considerato un limite definitivo.

# 17. Possibile secondo algoritmo: SHA-256

Nella discussione di ottobre è stata proposta una seconda direzione.

SHA-256 offre caratteristiche interessanti:

- algoritmo standardizzato;
- significato internazionale;
- vettori di test consolidati;
- complessità tecnica significativa;
- assenza di dipendenza da anagrafiche o codici territoriali;
- possibilità di confronto rigoroso tra implementazioni.

Per questa challenge potrebbe essere richiesta l'implementazione integrale di SHA-256, vietando l'utilizzo di librerie che realizzano già l'hashing.

Il compromesso è una maggiore difficoltà nei linguaggi storici e negli ambienti che non dispongono di operazioni efficienti su interi e bit.

**Proposta di separazione:**

`every-language-fiscal-code` — prima sfida e progetto pilota.

`every-language-sha256` — eventuale seconda sfida, particolarmente adatta a una dimostrazione algoritmica universale.

Non occorre costruire entrambe le challenge contemporaneamente.

Il tentativo Guinness potrebbe utilizzare quella meglio compatibile con le linee guida approvate.

# 18. Ruolo di Openapi

Openapi deve essere visibile come promotore dell'iniziativa, non come intermediario obbligatorio.

Il progetto:

- non deve richiedere registrazione commerciale;
- non deve dipendere dalle API di Openapi per essere eseguito;
- non deve contenere funzionalità artificiali create per pubblicizzare prodotti;
- deve essere libero di accogliere contributor esterni;
- deve avere una licenza open source chiara;
- deve continuare a essere utile anche se non si realizza il record.

Il ritorno per Openapi deriva dall'associazione del marchio a un'iniziativa aperta, tecnicamente interessante e capace di attrarre la comunità internazionale.

Openapi può finanziare o fornire:

- manutenzione della repository;
- infrastruttura CI;
- documentazione;
- review delle Pull Request;
- coordinamento dei contributor;
- comunicazione del progetto;
- eventuale gestione della pratica Guinness.

Il marchio Openapi potrà comparire nel README, nel sito e nella documentazione come promotore ufficiale.

# 19. Rischi e problemi aperti

## 19.1 Record non riconosciuto

Non esiste ancora una conferma ufficiale che la categoria proposta venga accettata.

Il rischio va gestito dall'inizio, senza promettere un Guinness prima dell'approvazione.

## 19.2 Precedenti

Rosetta Code e altri progetti di programmazione multilinguaggio rappresentano precedenti importanti.

Bisogna verificare che la sfida proponga qualcosa di oggettivamente distinto e misurabile.

## 19.3 Linguaggi non realmente distinti

Non si può aumentare artificialmente il conteggio attraverso versioni, alias e dialetti sostanzialmente equivalenti.

## 19.4 Codice non indipendente

Ogni implementazione deve contenere la logica effettiva dell'algoritmo.

I wrapper non possono essere considerati implementazioni indipendenti.

## 19.5 Esecuzione in ambienti particolari

Alcuni linguaggi storici potrebbero richiedere emulatori, sistemi operativi specializzati o toolchain non facilmente disponibili.

La repository deve conservare le istruzioni e, quando possibile e legalmente consentito, consentire la riproduzione dell'ambiente di esecuzione.

## 19.6 Manutenzione

Le implementazioni potrebbero smettere di compilare dopo aggiornamenti del sistema o dei compilatori.

È necessario identificare la differenza fra implementazioni storicamente verificate e implementazioni ancora verificabili nella CI corrente.

## 19.7 Differenze normative e anagrafiche

Il codice fiscale italiano include regole, eccezioni e dati territoriali che devono essere trattati correttamente.

La specifica dovrà chiarire il significato esatto di "calcolare", "validare" e "verificare" un codice fiscale.

# 20. Principi non negoziabili

Il progetto deve seguire queste regole:

1. Il codice sorgente delle implementazioni è pubblico.
2. Esiste una sola specifica di comportamento per ciascuna challenge.
3. Tutti i linguaggi superano la stessa suite obbligatoria.
4. Nessuna implementazione viene conteggiata senza un'esecuzione verificata.
5. Ogni linguaggio viene classificato secondo criteri trasparenti.
6. Non si conteggiano wrapper che delegano l'algoritmo a un altro linguaggio.
7. Le prove tecniche devono essere riproducibili.
8. Il numero di linguaggi validi viene calcolato automaticamente.
9. I risultati non vengono presentati come certificati Guinness senza riconoscimento ufficiale.
10. La partecipazione deve essere aperta e indipendente dai servizi commerciali di Openapi.

# 21. Primo deliverable

Il primo risultato concreto non è una repository con 100 linguaggi.

È una repository con:

- specifica dell'algoritmo stabile;
- suite di test condivisa;
- runner centralizzato;
- manifest standard;
- 5–10 implementazioni realmente differenti;
- workflow CI funzionante;
- README e guida per contributor;
- report JSON automaticamente generato;
- dashboard di avanzamento;
- documento che descrive la proposta di record.

Questa base consentirà successivamente di aumentare il numero di linguaggi senza modificare l'architettura.

# 22. Definizione sintetica del progetto

**Every Language Challenge** è un'iniziativa open source promossa da Openapi con l'obiettivo di implementare lo stesso algoritmo nel maggior numero possibile di linguaggi di programmazione.

La prima challenge riguarda il calcolo del codice fiscale italiano.

Ogni contributore può realizzare un'implementazione indipendente nel linguaggio che preferisce, rispettando una specifica comune e superando test automatici condivisi.

Il progetto parte dall'obiettivo di 100 linguaggi, con un'architettura capace di raggiungerne almeno 256.

L'ambizione è trasformare la raccolta in un tentativo Guinness World Records basato su criteri oggettivi di esecuzione, riproducibilità e verifica.

Il risultato atteso è una biblioteca pubblica di implementazioni funzionanti, una comunità internazionale di contributor e una dimostrazione concreta della varietà dei linguaggi di programmazione.

**One algorithm. Every language. Every implementation verified.**

---

# 23. Istruzioni per l'avvio dello sviluppo locale

Utilizza questo documento come specifica iniziale del progetto.

Ordine di lavoro raccomandato:

1. Crea la struttura minima di `every-language-fiscal-code`.
2. Scrivi `SPECIFICATION.md` con le regole complete del codice fiscale italiano, verificandole sulle fonti normative.
3. Fissa il contratto input/output e la gestione degli errori.
4. Costruisci i vettori di test indipendenti.
5. Implementa il calcolo in PHP e Python.
6. Costruisci il runner comune, preferibilmente come una semplice CLI.
7. Aggiungi C, Rust e un linguaggio di paradigma differente come Prolog.
8. Integra la CI.
9. Genera un report con linguaggi validi, test ed evidenze.
10. Pubblica il primo README per la community.
11. Prepara separatamente la proposta di categoria Guinness.
12. Espandi la challenge tramite contributor e Pull Request.

Non iniziare generando automaticamente 100 cartelle vuote o 100 implementazioni non verificate.

La difficoltà principale non è scrivere cento volte lo stesso algoritmo: è costruire un sistema nel quale cento programmi diversi possano essere eseguiti, confrontati e riconosciuti come implementazioni autentiche.

**Criterio di successo della prima milestone:** dieci linguaggi realmente differenti, dieci implementazioni indipendenti, una suite condivisa completamente superata e un unico report riproducibile.

Questo costituisce la base tecnica dell'intero progetto.