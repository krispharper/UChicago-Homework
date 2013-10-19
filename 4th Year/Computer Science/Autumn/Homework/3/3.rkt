;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname |3|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Kris Harper
;; CMSC 15100
;; 10.19.10
;; Homework 3

;; Exercise 1

;; An expression is either
;; - a number or
;; - a bexp

(define-struct bexp
  (op ; : symbol
   left ; :expression
   right ; : expression
   ))

;; expression->string : expression -> string
;; returns the fully-parenthesized string representation of e
;; (define (expression->string e) ...)

(define (expression->string e)
  (cond
    [(number? e) (number->string e)]
    [(not (bexp? e)) (error 'expression->string "bexp expected")]
    [else (string-append
           "("
           (expression->string (bexp-left e))
           " "
           (symbol->string (bexp-op e))
           " "
           (expression->string (bexp-right e))
           ")")]))

(check-expect (expression->string 0) "0")
(check-expect (expression->string (make-bexp '* (make-bexp '+ 1 2) 3)) "((1 + 2) * 3)")
(check-error (expression->string "test") "expression->string: bexp expected")


;; Exercise 2

;; a ftn (family-tree node) is either
;; - 'unknown, or
;; - a child
;;
;; where

(define-struct child
  (name ; : string -- the child's name
   date ; : number -- the child's age
   mom ; : ftn -- the child's mother
   dad)) ; : ftn -- the child's father

;; a family tree node for check-expects

(define test-ftn
  (make-child "John" 20
              (make-child "Jane" 21
                          (make-child "Jenny" 31
                                      'unknown
                                      (make-child "Jake" 22 'unknown 'unknown))
                          (make-child "Jack" 28
                                      (make-child "Jill" 25 'unknown 'unknown)
                                      'unknown))
              (make-child "James" 29 'unknown
                          (make-child "Jimmy" 30 'unknown 'unknown))))


;; 30-year-old? : ftn -> boolean
;; to determine if a 30-year-old person is in a-ftn
;; (define (30-year-old? a-ftn) ...)

(define (30-year-old? a-ftn)
  (cond
    [(symbol? a-ftn) (cond
                       [(not (symbol=? 'unknown a-ftn)) (error '30-year-old? "ftn expected")]
                       [else #f])]
    [else (cond
            [(not (child? a-ftn)) (error '30-year-old? "ftn expected")]
            [else (cond
                    [(= 30 (child-date a-ftn)) #t]
                    [else (or (30-year-old? (child-mom a-ftn)) (30-year-old? (child-dad a-ftn)))])])]))

(check-expect (30-year-old? 'unknown) #f)
(check-expect (30-year-old? test-ftn) #t)
(check-expect (30-year-old? (make-child "" 29 'unknown 'unknown)) #f)
(check-error (30-year-old? 'test) "30-year-old?: ftn expected")
(check-error (30-year-old? 0) "30-year-old?: ftn expected")

;; count-people : ftn -> integer
;; to count the number of people in a-ftn
;; (define (count-people a-ftn) ...)

(define (count-people a-ftn)
  (cond
    [(symbol? a-ftn) (cond
                       [(not (symbol=? 'unknown a-ftn)) (error 'count-people "ftn expected")]
                       [else 0])]
    [else (cond
            [(not (child? a-ftn)) (error 'count-people "ftn expected")]
            [else (+ 1 (count-people (child-mom a-ftn)) (count-people (child-dad a-ftn)))])]))

(check-expect (count-people 'unknown) 0)
(check-expect (count-people test-ftn) 8)
(check-error (count-people 'test) "count-people: ftn expected")
(check-error (count-people 0) "count-people: ftn expected")

;; age-by-1 : ftn -> ftn
;; add 1 to the age of every person in a-ftn
;; (define (age-by-1 a-ftn) ...)

(define (age-by-1 a-ftn)
  (cond
    [(symbol? a-ftn) (cond
                       [(not (symbol=? 'unknown a-ftn)) (error 'age-by-1 "ftn expected")]
                       [else 'unknown])]
    [else (cond
            [(not (child? a-ftn)) (error 'age-by-1 "ftn expected")]
            [else (make-child
                   (child-name a-ftn)
                   (+ 1 (child-date a-ftn))
                   (age-by-1 (child-mom a-ftn))
                   (age-by-1 (child-dad a-ftn)))])]))

(check-expect (age-by-1 'unknown) 'unknown)
(check-expect (age-by-1 (make-child "" 30 'unknown 'unknown)) (make-child "" 31 'unknown 'unknown))
(check-expect (age-by-1 test-ftn) (make-child "John" 21
                                              (make-child "Jane" 22
                                                          (make-child "Jenny" 32
                                                                      'unknown
                                                                      (make-child "Jake" 23 'unknown 'unknown))
                                                          (make-child "Jack" 29
                                                                      (make-child "Jill" 26 'unknown 'unknown)
                                                                      'unknown))
                                              (make-child "James" 30 'unknown
                                                          (make-child "Jimmy" 31 'unknown 'unknown))))
(check-error (age-by-1 'test) "age-by-1: ftn expected")
(check-error (age-by-1 0) "age-by-1: ftn expected")

;; sum-age : ftn -> number
;; sums the ages of children in an ftn
;; (define (sum-age a-ftn) ...)

(define (sum-age a-ftn)
  (cond
    [(symbol? a-ftn) (cond
                       [(not (symbol=? 'unknown a-ftn)) (error 'sum-age "ftn expected")]
                       [else 0])]
    [else (cond
            [(not (child? a-ftn)) (error 'sum-age "ftn expected")]
            [else (+ (child-date a-ftn) (sum-age (child-mom a-ftn)) (sum-age (child-dad a-ftn)))])]))

(check-expect (sum-age 'unknown) 0)
(check-expect (sum-age test-ftn) 206)
(check-error (sum-age 'test) "sum-age: ftn expected")
(check-error (sum-age 0) "sum-age: ftn expected")


;; average-age : ftn -> number
;; to determine the average age in a-ftn
;; (define (average-age a-ftn) ...)

(define (average-age a-ftn)
  (cond
    [(symbol? a-ftn) (cond
                       [(not (symbol=? 'unknown a-ftn)) (error 'average-age "ftn expected")]
                       [else 0])]
    [else (cond
            [(not (child? a-ftn)) (error 'average-age "ftn expected")]
            [else (/ (+ (child-date a-ftn) (sum-age (child-mom a-ftn)) (sum-age (child-dad a-ftn))) (count-people a-ftn))])]))

(check-expect (average-age 'unknown) 0)
(check-expect (average-age test-ftn) 25.75)
(check-error (average-age 'test) "average-age: ftn expected")
(check-error (average-age 0) "average-age: ftn expected")