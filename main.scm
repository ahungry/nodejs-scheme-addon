;; The eventual return value needs to be a string for proper
;; type support.   Otherwise, leave the casting to the GDScript side.
(define (all-strings s)
  (if (string? s) s (number->string s)))

;; Uncomment this to run a REPL for Geiser to connect to
(use-modules (system repl server))
(spawn-server (make-tcp-server-socket #:port 37146))

(my-fn "fake" "Greetings from inside your lisp implementation")
(my-fn "fake" "I am still here...")

(my-fn "fake" (string-append "MGR1: " (my-get-register)))
(my-fn "fake" (my-set-register "SET-REGISTER-in-SCHEME"))
(my-fn "fake" (string-append "MGR2: " (my-get-register)))

;; Set up a construct where we mark a node request as pending and halt until it resolves.
(define (with-sync f)
  (my-set-register "<PENDING>")
  (while (not (equal? "<PENDING>" (my-get-register)))
    (usleep 100))
  (f)
  (while (equal? "<PENDING>" (my-get-register))
    (usleep 100))
  (my-get-register))

;; Test out an async call with a scheme side block.
(define (fetch-with-pause)
  (with-sync (lambda () (my-fn "x" "y"))))

(all-strings (my-sum  20 30))
