;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname |4|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Kris Harper
;; CMSC 15100
;; 10.26.10
;; Homework 4

;; Exercise 1

;; A bst (binary-search tree) is either
;; - a leaf represented by 'leaf, or
;; - a bst-node

(define-struct bst-node
  (val ; : number
   left ; : bst
   right ; : bst
   ))

;; some bsts for check-expects

(define bst1 (make-bst-node 2 (make-bst-node 1 'leaf 'leaf) (make-bst-node 3 'leaf 'leaf)))

(define bst2 (make-bst-node 4
                            (make-bst-node 2
                                           (make-bst-node 1 'leaf 'leaf)
                                           (make-bst-node 3 'leaf 'leaf))
                            (make-bst-node 6
                                           (make-bst-node 5 'leaf 'leaf)
                                           (make-bst-node 7 'leaf 'leaf))))

;; Member? : bst number -> boolean
;; returns #t if num is in bst
;; (define (Member? bst num) ...)

(define (Member? bst num)
  (cond
    [(equal? bst 'leaf) #f]
    [(< num (bst-node-val bst)) (Member? (bst-node-left bst) num)]
    [(> num (bst-node-val bst)) (Member? (bst-node-right bst) num)]
    [else #t]))

;; inorder : bst -> (sorted-list number)
;; returns the elements of bst in ascending order
;; (define (inorder bst) ...)

(define (inorder bst)
  (cond 
    [(not (or (equal? 'leaf bst) (bst-node? bst))) (error 'inorder "bst expected")]
    [else (cond
            [(equal? bst 'leaf) empty]
            [else (append
                   (inorder (bst-node-left bst))
                   (list (bst-node-val bst))
                   (inorder (bst-node-right bst)))])]))

(check-expect (inorder 'leaf) '())
(check-expect (inorder bst1) '(1 2 3))
(check-expect (inorder bst2) '(1 2 3 4 5 6 7))
(check-error (inorder 0) "inorder: bst expected")

;; insert : bst number -> bst
;; returns a BST that is bst with num added to it.
;; (define (insert bst num) ...)

(define (insert bst num)
  (cond 
    [(not (or (equal? 'leaf bst) (bst-node? bst))) (error 'insert "bst expected")]
    [(not (number? num)) (error 'insert "number expected")]
    [else (cond
            [(equal? 'leaf bst) (make-bst-node num 'leaf 'leaf)]
            [(< num (bst-node-val bst)) (make-bst-node
                                         (bst-node-val bst)
                                         (insert (bst-node-left bst) num)
                                         (bst-node-right bst))]
            [(> num (bst-node-val bst)) (make-bst-node
                                         (bst-node-val bst)
                                         (bst-node-left bst)
                                         (insert (bst-node-left bst) num))]
            [else bst])]))

(check-expect (insert 'leaf 1) (make-bst-node 1 'leaf 'leaf))
(check-expect
 (insert (make-bst-node 1 'leaf 'leaf) 1)
 (make-bst-node 1 'leaf 'leaf))
(check-expect
 (insert (make-bst-node 1 'leaf 'leaf) 0)
 (make-bst-node 1 (make-bst-node 0 'leaf 'leaf) 'leaf))
(check-expect
 (insert (make-bst-node 1 'leaf 'leaf) 2)
 (make-bst-node 1 'leaf (make-bst-node 2 'leaf 'leaf)))
(check-error (insert 0 0) "insert: bst expected")
(check-error (insert 'leaf 'leaf) "insert: number expected")

;; Exercise 2

;; A heap is either
;; - a leaf represented by 'leaf, or
;; - a heap-node

(define-struct heap-node
  (val ; : number
   left ; : heap
   right ; : heap
   ))

;; some heaps for check-expects

(define heap1 (make-heap-node 0 'leaf 'leaf))
(define heap2 (make-heap-node 0
                              (make-heap-node 1 'leaf 'leaf)
                              (make-heap-node 2 'leaf 'leaf)))
(define heap3 (make-heap-node 0
                              (make-heap-node 0 'leaf 'leaf)
                              (make-heap-node 0 'leaf 'leaf)))

;; least : heap -> number
;; returns the smallest element in the heap, or else #f for an empty heap
;; (define (least heap) ...)

(define (least h)
  (cond
    [(not (or (equal? 'leaf h) (heap-node? h))) (error 'least "heap expected")]
    [else (cond
            [(equal? 'leaf h) #f]
            [else (heap-node-val h)])]))

(check-expect (least 'leaf) #f)
(check-expect (least heap1) 0)
(check-expect (least heap2) 0)
(check-expect (least heap3) 0)
(check-error (least 0) "least: heap expected")

;; heap-insert : heap number -> heap
;; inserts the number into the heap
;; (define (heap-insert heap) ...)

(define (heap-insert h n)
  (cond
    [(not (or (equal? 'leaf h) (heap-node? h))) (error 'heap-insert "heap expected")]
    [(not (number? n)) (error 'heap-insert "number expected")]
    [else (cond
            [(equal? 'leaf h) (make-heap-node n 'leaf 'leaf)]
            [(<= n (heap-node-val h)) (make-heap-node n 'leaf h)]
            [else (make-heap-node
                   (heap-node-val h)
                   (heap-node-left h)
                   (heap-insert (heap-node-right h) n))])]))

(check-expect (heap-insert 'leaf 0) (make-heap-node 0 'leaf 'leaf))
(check-expect (heap-insert heap1 0) (make-heap-node 0 'leaf heap1))
(check-expect (heap-insert heap2 3) (make-heap-node 0
                                                    (make-heap-node 1 'leaf 'leaf)
                                                    (make-heap-node 2'leaf (make-heap-node 3 'leaf 'leaf))))
(check-error (heap-insert 0 0) "heap-insert: heap expected")
(check-error (heap-insert 'leaf 'leaf) "heap-insert: number expected")