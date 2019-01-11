;; Simple scheme two add two nums

(define n1 (string->number (my-get-register)))
(define n2 (string->number (my-get-register)))

(number->string (+ n1 n2))
