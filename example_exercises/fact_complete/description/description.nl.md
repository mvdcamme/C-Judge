Maak een functie om de faculteit van een getal te berekenen.
In Scheme kan je die functie bv. als volgt definiëren:
```
(define (fac n)
  (if (= n 0)
      1
      (* n (fac (- n 1)))))
```

Vertaal deze functie nu naar C.
Laat de functie een integer-type als argument nemen en een integer-type teruggeven: implementeer m.a.w. de functie `int fac(int n`).
