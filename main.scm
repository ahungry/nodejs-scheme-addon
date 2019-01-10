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

(all-strings (my-sum  20 30))
