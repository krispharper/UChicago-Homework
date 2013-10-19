;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname |4|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Kris Harper
;; Lab 4
;; CMSC 15100
;; 10.24.10

(require 2htdp/image)
(require 2htdp/universe)
(require htdp/matrix)
(require 2htdp/batch-io)


;; Part 1

;; make-letter-grid : num num -> matrix-of-char
;; makes a random matrix of letters with every third letter a vowel
;; (define (make-letter-grid n m)...)

(define (make-letter-grid n m)
  (cond
    [(or (not (integer? n)) (not (integer? m))) (error 'make-letter-grid "integer expected")]
    [else (local ((define (make-list l a)
                    (cond
                      [(= (length l) a) l]
                      [else (cond
                              [(integer? (/ (+ (length l) 1) 3))
                               (make-list
                                (append
                                 l
                                 (list (string-ref "AEIOU" (random 5))))
                                a)]
                              [else
                               (make-list
                                (append
                                 l
                                 (list (integer->char (+ (random 26) 65))))
                                a)])])))
            (make-matrix n m (make-list empty (* n m))))]))

(check-expect (matrix? (make-letter-grid 4 4)) #t)
(check-error (make-letter-grid 1.2 8) "make-letter-grid: integer expected")


;; Part 2

;; some paths for check-expects

(define path1 (list (make-posn 1 1) (make-posn 1 2) (make-posn 2 3)))
(define path2 (list (make-posn 1 1) (make-posn 1 2) (make-posn 2 4)))
(define path3 (list (make-posn 1 1) (make-posn 1 2) (make-posn 1 1)))

;; valid-path? : list-of-posn -> boolean
;; checks if a path is contiguous and free from cycles
;; (define (valid-path? l)...)

(define (valid-path? l)
  (cond
    [(not (list? l)) (error 'valid-path? "list expected")]
    [else (cond
            [(empty? l) #t]
            [(empty? (rest l)) #t]
            [(or (> (abs (- (posn-x (first l)) (posn-x (first (rest l))))) 1)
                 (> (abs (- (posn-y (first l)) (posn-y (first (rest l))))) 1))
             #f]
            [(member? (first l) (rest l)) #f]
            [else (valid-path? (rest l))])]))

(check-expect (valid-path? empty) #t)
(check-expect (valid-path? path1) #t)
(check-expect (valid-path? path2) #f)
(check-expect (valid-path? path3) #f)
(check-error (valid-path? 0) "valid-path?: list expected")

;; path->word : list-of-posn matrix-of-char -> string
;; which converts a path in a grid into a word
;; (define (path->word l m)...)

(define (path->word l m)
  (cond
    [(not (list? l)) (error 'path->word "list expected")]
    [(not (matrix? m)) (error 'path->word "matrix expected")]
    [else (cond
            [(empty? l) ""]
            [else (string-append
                   (string (matrix-ref m (posn-x (first l)) (posn-y (first l))))
                   (path->word (rest l) m))])]))

(check-expect (string? (path->word path1 (make-letter-grid 4 4))) #t)
(check-expect (path->word empty (make-letter-grid 4 4)) "")
(check-error (path->word 0 (make-letter-grid 4 4)) "path->word: list expected")
(check-error (path->word path1 0) "path->word: matrix expected")

;; score-string : string -> num
;; assigns a score to a word with 2 points for a consonant and 1 point for a vowel
;; (define (score-string s)...)

(define (score-string s)
  (cond
    [(not (string? s)) (error 'score-string "string expected")]
    [else (cond
            [(string=? s "") 0]
            [else (+ (cond
                       [(member? (string-ref s 0) (string->list "AEIOU")) 1]
                       [else 2]) (score-string (substring s 1)))])]))

(check-expect (number? (score-string "TEST")) #t)
(check-expect (score-string "") 0)
(check-expect (score-string "TEST") 7)
(check-error (score-string 0) "score-string: string expected")


;; Part 3

;; a world is a (make-world grid list path mess score), where
;; - grid is a matrix of chars
;; - list is a list of strings
;; - path is a list of posns
;; - mess is a string
;; - score is an integer

(define-struct world (grid list path mess score))

;; render-grid : matrix -> img
;; renders a matrix of characters as an image
;; (define (render-grid m)...)

(define (render-grid m)
  (cond
    [(not (matrix? m)) (error 'render-grid "matrix expected")]
    [else (local ((define (rows l)
                    (cond
                      [(empty? l) (rectangle 0 0 "solid" "white")]
                      [else (beside (overlay (text (string (first l)) 20 "maroon")
                                             (rectangle 30 30 "outline" "black"))
                                    (rows (rest l)))]))
                  (define (cols l)
                    (cond
                      [(empty? l) (rectangle 0 0 "solid" "white")]
                      [else (above (rows (first l))
                                   (cols (rest l)))])))
            (cols (matrix->rectangle m)))]))

(check-expect (image? (render-grid (make-letter-grid 4 4))) #t)
(check-expect (image-width (render-grid (make-letter-grid 4 4))) 120)
(check-error (render-grid 0) "render-grid: matrix expected")

;; render-words : list-of-string -> img
;; render a list of words one below the other in 12 point font
;; (define (render-words l)...

(define (render-words l)
  (cond
    [(not (list? l)) (error 'render-words "list expected")]
    [else (cond
            [(empty? l) (rectangle 0 0 "solid" "white")]
            [else (above/align "left" (text (first l) 12 "black")
                               (render-words (rest l)))])]))

(check-expect (image? (render-words (list "TEST"))) #t)
(check-error (render-words 0) "render-words: list expected")

;; render : world -> scene
;; draws the appropriate scene given a world
;; (define (render w)...)

(define (render w)
  (cond
    [(not (world? w)) (error 'render "world expected")]
    [else (place-image/align
           (beside/align "top"
                         (render-grid (world-grid w))
                         (overlay/xy
                          (rectangle 12 10 "solid" "white")
                          0 -1
                          (rectangle 10 10 "outline" "black"))
                         (above/align "left"
                                      (text (string-append
                                             "Score: "
                                             (number->string (world-score w)))
                                            13
                                            "red")
                                      (text (world-mess w) 13 "blue")
                                      (render-words (world-list w))))
           0 0 "left" "top"
           (empty-scene
            (+ (image-width (render-grid (world-grid w))) 200)
            (+ (image-height (render-grid (world-grid w))) 100)))]))

(check-expect (image? (render (make-world (make-letter-grid 4 4)
                                          (list "TEST" "TESTER" "TESTING")
                                          path1
                                          "THIS IS A MESSAGE"
                                          15))) #t)
(check-error (render 0) "render: world expected")


;; Part 4

(define dictionary (read-words "words.txt"))

;; add-word : world -> world
;; converts the current path into a word
;; (define (add-word w)...)

(define (add-word w)
  (cond
    [(not (world? w)) (error 'add-word "world expected")]
    [else (cond
            [(and
              (valid-path? (world-path w))
              (member? (path->word (world-path w) (world-grid w)) dictionary)
              (not (member? (path->word (world-path w) (world-grid w)) (world-list w))))
             (make-world (world-grid w)
                         (append (world-list w) (list (path->word (world-path w) (world-grid w))))
                         empty
                         (string-append
                          "+ "
                          (number->string (score-string (path->word (world-path w) (world-grid w))))
                          " points for "
                          (path->word (world-path w) (world-grid w)))
                         (+ (world-score w)
                            (score-string
                             (path->word (world-path w) (world-grid w)))))]
            [else (make-world (world-grid w)
                              (world-list w)
                              empty
                              (string-append
                               "- "
                               (number->string (score-string (path->word (world-path w) (world-grid w))))
                               " points for invalid word")
                              (- (world-score w)
                                 (score-string
                                  (path->word (world-path w) (world-grid w)))))])]))

(check-expect (world? (add-word (make-world (make-letter-grid 4 4)
                                            (list "TEST" "TESTER")
                                            path1
                                            ""
                                            20))) #t)
(check-error (add-word 0) "add-word: world expected")

;; read-mouse : world num num mouse-event -> world
;; reads mouse events and acts accordingly
;; (define (read-mouse w n m e)...)

(define (read-mouse w n m e)
  (cond
    [(not (world? w)) (error 'read-mouse "world expected")]
    [(or (not (integer? n)) (not (integer? m))) (error 'read-mouse "integer expected")]
    [else (cond
            [(mouse=? e "button-up")
             (cond
               [(and
                 (<= n (image-width (render-grid (world-grid w))))
                 (<= m (image-height (render-grid (world-grid w)))))
                (make-world (world-grid w)
                            (world-list w)
                            (append (world-path w)
                                    (list (make-posn
                                           (floor (/ m 30))
                                           (floor (/ n 30)))))
                            (path->word (append (world-path w)
                                                (list (make-posn
                                                       (floor (/ m 30))
                                                       (floor (/ n 30)))))
                                        (world-grid w))
                            (world-score w))]
               [else (add-word w)])]
            [else w])]))

(check-error (read-mouse 0 0 0 0) "read-mouse: world expected")
(check-error (read-mouse
              (make-world (make-letter-grid 4 4) empty empty "" 0)
              1.2 0 0)
             "read-mouse: integer expected")

;; init-world : world
;; the initial world for the game

(define init-world
  (make-world (make-letter-grid 4 4) empty empty "New Game" 0))

;; call the start of the game

(big-bang init-world (to-draw render) (on-mouse read-mouse))