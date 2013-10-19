;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname |5|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;; Kris Harper
;; Lab 5
;; CMSC 15100
;; 11.10.10

;; data definitions

;; a (pair-of α β) is a (make-pair-of a b), where
;; a is an α and b is a β
(define-struct pair-of (item1 item2))

;; a bit is either 0 or 1

;; a huff is either a leaf or a node, where

;; a leaf is a (make-leaf count char path-to), where
;; count is a num, char is a char, and path-to is list of bits
(define-struct leaf (count char path-to))

;; a node is a (make-node count left right), where
;; count is a num, and left and right are huffs
(define-struct node (count left right))


;; Part 1: Construct the Tree

;; pair : string -> (list of pair-of)
;; given a message, generate a list of character/number pairs, where
;;  each number is the occurrence count of that character
;; (define (pair mess)...)

(define (pair mess)
  (cond
    [(not (string? mess)) (error 'pair "string expected")]
    [else (local
            ((define (count l)
               (cond
                 [(empty? l) l]
                 [else (cons
                        (make-pair-of
                         (pair-of-item1 (first l))
                         (length
                          (filter
                           (lambda (x)
                             (equal? (pair-of-item1 x)
                                     (pair-of-item1 (first l))))
                           l)))
                        (count
                         (filter
                          (lambda (x)
                            (not (equal? (pair-of-item1 x)
                                         (pair-of-item1 (first l)))))
                          l)))])))
            (count (map
                    (lambda (x) (make-pair-of x 1))
                    (string->list mess))))]))

(check-expect (list? (pair "TEST")) #t)
(check-expect (pair "") empty)
(check-expect (pair "TEST") (list
                             (make-pair-of #\T 2)
                             (make-pair-of #\E 1)
                             (make-pair-of #\S 1)))
(check-error (pair 0) "pair: string expected")

;; make-singles : (list of pair-of) -> (list of leafs)
;; given a list of char/num pairs, produce the sorted list of "singleton trees"
;; (define (make-singles l)...)

(define (make-singles l)
  (cond
    [(not (list? l)) (error 'make-singles "list expected")]
    [else (quicksort
           (map
            (lambda (x) (make-leaf (pair-of-item2 x)
                                   (pair-of-item1 x)
                                   (cond
                                     [(= (length l) 1) (list 0)]
                                     [else empty])))
            l)
           (lambda (x y) (< (leaf-count x)
                            (leaf-count y))))]))

(check-expect (list? (make-singles (pair "TEST"))) #t)
(check-expect (make-singles empty) empty)
(check-expect (make-singles (pair "TEST")) (list
                                            (make-leaf 1 #\E empty)
                                            (make-leaf 1 #\S empty)
                                            (make-leaf 2 #\T empty)))
(check-error (make-singles 0) "make-singles: list expected")

;; insert : (list of huffs) huff -> (list of huffs)
;; takes a huff and inserts it into a list of huffs
;; (define (insert l h)...)

(define (insert l h)
  (cond
    [(not (list? l)) (error 'insert "list expected")]
    [else (local ((define val
                    (cond
                      [(node? h) (node-count h)]
                      [(leaf? h) (leaf-count h)])))
            (append
             (filter
              (lambda (x)
                (cond
                  [(node? x) (< (node-count x) val)]
                  [(leaf? x) (< (leaf-count x) val)])) l)
             (list h)
             (filter
              (lambda (x)
                (cond
                  [(node? x) (>= (node-count x) val)]
                  [(leaf? x) (>= (leaf-count x) val)])) l)))]))

(check-expect (insert
               (list (make-leaf 1 #\A empty)
                     (make-leaf 3 #\B empty))
               (make-leaf 2 #\C empty))
              (list (make-leaf 1 #\A empty)
                    (make-leaf 2 #\C empty)
                    (make-leaf 3 #\B empty)))
(check-error (insert 0 (make-leaf 0 #\A empty)) "insert: list expected")

;; make-paths : huff (list of bits) -> huff
;; update the paths at the leafs of a huff
;; Note that Adam Shaw said I could use this method instead of the one described
;; (define (make-paths h l)...)

(define (make-paths h l)
  (cond
    [(not (list? l)) (error 'make-paths "list expected")]
    [else
     (cond
       [(leaf? h) (make-leaf (leaf-count h)
                             (leaf-char h)
                             (cond
                               [(empty? (leaf-path-to h)) (reverse l)]
                               [else (leaf-path-to h)]))]
       [else (make-node (node-count h)
                        (make-paths (node-left h) (cons 0 l))
                        (make-paths (node-right h) (cons 1 l)))])]))

(check-expect (make-paths (make-leaf 0 #\A empty) empty) (make-leaf 0 #\A empty))
(check-expect (make-paths (make-node
                           2
                           (make-leaf 1 #\A empty)
                           (make-leaf 1 #\B empty)) empty)
              (make-node
               2
               (make-leaf 1 #\A (list 0))
               (make-leaf 1 #\B (list 1))))
(check-error (make-paths (make-leaf 0 #\A empty) 0) "make-paths: list expected")


;; make-tree : (list of huffs) -> huff
;; make a huff given a list of leafs
;; (define (make-tree l)...)

(define (make-tree l)
  (cond
    [(not (list? l)) (error 'make-tree "list expected")]
    [else (cond
            [(empty? (rest l)) (first l)]
            [else (local ((define val1
                            (cond
                              [(node? (first l)) (node-count (first l))]
                              [(leaf? (first l)) (leaf-count (first l))]))
                          (define val2
                            (cond
                              [(node? (second l)) (node-count (second l))]
                              [(leaf? (second l)) (leaf-count (second l))])))
                    (make-paths
                     (make-tree
                      (remove (second l)
                              (remove (first l)
                                      (insert
                                       l
                                       (make-node
                                        (+ val1 val2)
                                        (first l)
                                        (second l)))))) empty))])]))

(check-expect (make-tree (make-singles (pair "TEST")))
              (make-node 4
                         (make-node
                          2
                          (make-leaf 1 #\E (list 0 0))
                          (make-leaf 1 #\S (list 0 1)))
                         (make-leaf 2 #\T (list 1))))
(check-error (make-tree 0) "make-tree: list expected")


;; Part 2: Decoding

;; decode : (list-of-bits) huff -> string
;; decodes a given encoded message
;; (define (decode l h)...)

(define (decode l h)
  (cond
    [(not (list? l)) (error 'decode "list expected")]
    [else (local
            ((define main h)
             (define (decode-aux l h s)
               (cond
                 [(empty? l) s]
                 [else
                  (cond
                    [(= 0 (first l))
                     (cond
                       [(leaf? h)
                        (decode-aux
                         (rest l)
                         main
                         (string-append (string (leaf-char h)) s))]
                       [(leaf? (node-left h))
                        (decode-aux
                         (rest l)
                         main
                         (string-append (string (leaf-char (node-left h))) s))]
                       [else (decode-aux (rest l) (node-left h) s)])]
                    [(= 1 (first l))
                     (cond
                       [(leaf? (node-right h))
                        (decode-aux
                         (rest l)
                         main
                         (string-append (string (leaf-char (node-right h))) s))]
                       [else (decode-aux (rest l) (node-right h) s)])])])))
            (list->string
             (reverse
              (string->list
               (decode-aux l h "")))))]))

(check-expect (decode
               (list 0 1 0 1 1 0 1 1)
               (make-tree (make-singles (pair "SHESELLSSEASHELLS"))))
              "EELS")
(check-error (decode 0 (make-leaf 0 #\A empty)) "decode: list expected")


;; Part 3: Encoding

;; encode : string -> (pair-of list-of-bits huff)
;; encode a plaintext message and return both the bitstring and
;;  the Huffman tree corresponding to that encoding
;; (define (encode s)...)

(define (encode s)
  (cond
    [(not (string? s)) (error 'encode "string expected")]
    [else (local ((define tree (make-tree (make-singles (pair s))))
                  (define (encode-aux s h)
                    (cond
                      [(string=? "" s) empty]
                      [else
                       (append
                        (cond
                          [(leaf? h)
                           (cond
                             [(char=? (string-ref s 0) (leaf-char h))
                              (append
                               (leaf-path-to h)
                               (encode-aux (substring s 1) tree))]
                             [else empty])]
                          [else (append
                                 (encode-aux s (node-left h))
                                 (encode-aux s (node-right h)))]))])))
            (make-pair-of (encode-aux s tree) tree))]))

(check-expect (pair-of? (encode "TEST")) #t)
(check-expect (encode "TEST") (make-pair-of
                               (list 1 0 0 0 1 1)
                               (make-node 4
                                          (make-node
                                           2
                                           (make-leaf 1 #\E (list 0 0))
                                           (make-leaf 1 #\S (list 0 1)))
                                          (make-leaf 2 #\T (list 1)))))
(check-error (encode 0) "encode: string expected")

;; round-trip : string -> string
;; encode and then decode a message
;; (define (round-trip s)...)

(define (round-trip msg)
  (local ((define p (encode msg)))
    (decode (pair-of-item1 p) (pair-of-item2 p))))

(check-expect (round-trip "TEST") "TEST")