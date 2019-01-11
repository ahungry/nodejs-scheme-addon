;; The eventual return value needs to be a string for proper
;; type support.   Otherwise, leave the casting to the GDScript side.
(define (all-strings s)
  (if (string? s) s (number->string s)))

;; Uncomment this to run a REPL for Geiser to connect to
(use-modules (system repl server))
(use-modules (ice-9 threads))
(use-modules (ice-9 futures))
(use-modules (ice-9 eval-string))
(use-modules (ice-9 regex))

(catch #t
  (lambda ()
    (spawn-server (make-tcp-server-socket #:port 37146)))
  (lambda (key . parameters)
    #nil))

;; (my-fn "fake" "Greetings from inside your lisp implementation")
;; (my-fn "fake" "I am still here...")
;; (my-fn "fake" (string-append "MGR1: " (my-get-register)))
;; (my-fn "fake" (my-set-register "SET-REGISTER-in-SCHEME"))
;; (my-fn "fake" (string-append "MGR2: " (my-get-register)))

;; Set up a construct where we mark a node request as pending and halt until it resolves.
(define-macro (await rest)
  `(begin
     (my-set-register "<PENDING>")
     (while (not (equal? "<PENDING>" (my-get-register)))
       (usleep 100))
     ,rest
     (while (equal? "<PENDING>" (my-get-register))
       (usleep 100))
     (my-get-register)))

;; Test out an async call with a scheme side block.
(define (fetch-with-pause)
  (await (my-fn "x" "y")))

(define (stub) "42")

(define (factorial n)
  (if (< n 2) n
      (* n (factorial (- n 1)))))

;; We can run a looping server to "listen" for node requests to run fns in here.
;; FIXME: This fails horribly with concurrent requests
(call-with-new-thread
 (lambda ()
   (while #t
     (usleep 10)
     (let [[maybe-eval (my-get-register)]]
       (if (string-match "^\\(" maybe-eval)
           (my-set-register (eval-string (my-get-register)))
           ;; else
           ;; (my-fn "x" (string-append "I am in the thread, look out!"
           ;;                           (my-get-register)))
           ))
     )))

(all-strings (my-sum 20 30))

;; (all-strings
;;  (fetch-with-pause))
