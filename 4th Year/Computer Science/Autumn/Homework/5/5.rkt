;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname |5|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;; Kris Harper
;; CMSC 15100
;; 12.1.10
;; Homework 5

;; Exercise 1

;; vector-map : (X -> Y) (vector X) -> (vector Y)
;; (define (vector-map f v)...)

(define (vector-map f v)
  (cond
    [(not (procedure? f)) (error 'vector-map "procedure expected")]
    [(not (vector? v)) (error 'vector-map "vector expected")]
    [else (build-vector (vector-length v) (lambda (n) (f (vector-ref v n))))]))

(check-expect (vector-map (lambda (x) (odd? x)) (vector 0 1 2 3)) (vector #f #t #f #t))
(check-error (vector-map 0 (vector 0)) "vector-map: procedure expected")
(check-error (vector-map (lambda (x) x) 0) "vector-map: vector expected")


;; Exercise 2

(define-struct deque
  (front ;; either a (deq-nd X) or #f
   rear ;; either a (deq-nd X) or #f
   ))

(define-struct deq-nd
  (val ; the node's value (type X)
   pred ; the node's successor; either a deq-nd or #f
   succ ; the node's predecessor; either a deq-nd or #f
   ))

;; some deques for check-expects

(define deq1 (make-deque #f #f))
(define deq2 (make-deque (make-deq-nd 'A #f #f) (make-deq-nd 'A #f #f)))
(define A (make-deq-nd 'A #f #f))
(define B (make-deq-nd 'B A #f))
(define C (make-deq-nd 'C B #f))
(set-deq-nd-succ! A B)
(set-deq-nd-succ! B C)
(define deq3 (make-deque A C))

;; deque->list : (deque X) -> (list X)
;; returns a list of the items in deq in front to back order.
;; (define (deque->list deq)...)

(define (deque->list deq)
  (cond
    [(not (deque? deq)) (error 'deque->list "deque expected")]
    [else (local ((define (aux nd)
                    (cond
                      [(not (deq-nd? nd)) empty]
                      [else (cons (deq-nd-val nd) (aux (deq-nd-succ nd)))])))
            (aux (deque-front deq)))]))

(check-expect (deque->list deq1) empty)
(check-expect (deque->list deq2) (list 'A))
(check-expect (deque->list deq3) (list 'A 'B 'C))
(check-error (deque->list 0) "deque->list: deque expected")

;; deque-push : (deque X) X -> void
;; push an item onto the front of the deque
;; (define (deque-push deq item)...)

(define (deque-push deq item)
  (cond
    [(not (deque? deq)) (error 'deque-push "deque expected")]
    [else (cond
            [(not (deq-nd? (deque-front deq)))
             (begin
               (set-deque-front! deq (make-deq-nd item #f #f))
               (set-deque-rear! deq (make-deq-nd item #f #f)))]
            [else (begin
                    (set-deq-nd-pred! (deque-front deq)
                                      (make-deq-nd item
                                                   #f
                                                   (deque-front deq)))
                    (set-deque-front! deq (make-deq-nd item
                                                       #f
                                                       (deque-front deq)))
                    (cond
                      [(equal? (deq-nd-pred (deque-rear deq)) #f)
                       (set-deq-nd-pred! (deque-rear deq) (deque-front deq))]
                      [else (set-deq-nd-pred! (deque-rear deq) #f)])
                    )])]))

(check-expect (void? (deque-push (make-deque #f #f) 'A)) #t)
(check-expect (local ((define deq (make-deque #f #f)))
                (begin
                  (deque-push deq 'A)
                  (deque->list deq)))
              (list 'A))
(check-error (deque-push 0 0) "deque-push: deque expected")

;; deque-pop : (deque X) -> X
;; pop an item off of the front of the deque; returns #f if the
;; deque is empty
;; (define (deque-pop deq)...)

(define (deque-pop deq)
  (cond
    [(not (deque? deq)) (error 'deque-pop "deque expected")]
    [else (cond
            [(not (deq-nd? (deque-front deq))) #f]
            [else (local ((define val (deq-nd-val (deque-front deq))))
                    (begin
                      (cond
                        [(equal? (deq-nd-succ (deque-front deq)) #f)
                         (begin
                           (set-deque-front! deq #f)
                           (set-deque-rear! deq #f))]
                        [else
                         (begin
                           (set-deque-front! deq
                                             (deq-nd-succ (deque-front deq)))
                           (set-deq-nd-pred! (deque-front deq) #f))])
                      val))])]))

(check-expect (deque-pop (make-deque #f #f)) #f)
(check-expect (local ((define deq (make-deque #f #f)))
                (begin
                  (deque-push deq 'A)
                  (deque-pop deq2)))
              'A)
(check-error (deque-pop 0) "deque-pop: deque expected")

;; find-and-remove : (X -> boolean) (deque X) -> (oneof X #f)
;; find the first element of the deque that satisfies the predicate and
;; remove it from the deque. Return the removed item or else #f if there
;; is no item satisfying the predicate.
;; (define (find-and-remove pred? deq)...)

(define (find-and-remove pred? deq)
  (cond
    [(not (procedure? pred?)) (error 'find-and-remove "procedure expected")]
    [(not (deque? deq)) (error 'find-and-remove "deque expected")]
    [else (cond
            [(not (deq-nd? (deque-front deq))) #f]
            [else (local
                    ((define (aux nd pred?)
                       (cond
                         [(equal? nd #f) #f]
                         [(pred? (deq-nd-val nd))
                          (cond
                            [(equal? (deq-nd-pred nd) #f) (deque-pop deq)]
                            [(equal? (deq-nd-succ nd) #f)
                             (begin
                               (set-deque-rear! deq (deq-nd-pred nd))
                               (set-deq-nd-succ! (deque-rear deq) #f))]
                            [else
                             (begin
                               (set-deq-nd-succ! (deq-nd-pred nd)
                                                 (deq-nd-succ nd))
                               (set-deq-nd-pred! (deq-nd-succ nd)
                                                 (deq-nd-pred nd)))])]
                         [else (aux (deq-nd-succ nd) pred?)])))
                    (aux (deque-front deq) pred?))])]))

(check-expect (local ((define deq (make-deque #f #f)))
                (begin
                  (deque-push deq 'A)
                  (find-and-remove (lambda (x) (equal? x 'A)) deq)))
              'A)
(check-error (find-and-remove 0 deq1) "find-and-remove: procedure expected")
(check-error (find-and-remove (lambda (x) x) 0) "find-and-remove: deque expected")