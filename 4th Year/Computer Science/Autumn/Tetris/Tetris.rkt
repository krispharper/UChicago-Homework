;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname Tetris) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;           Tetris          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Kris Harper
;; 10.29.10

;; a basic tetris game
;; controls are
;;  - up: rotate shape
;;  - left: move shape left
;;  - right: move shape right
;;  - down: move shape down
;;  - p: pause game

(require 2htdp/image)
(require 2htdp/universe)

;; a single is a (make-single color pos) where
;; - color is a string
;; - pos is a list of positions

(define-struct single (color pos))

;; a world is a (make-world curr next board rows score over pause) where
;; - curr is a single
;; - next is a single
;; - board is a list of singles
;; - rows is an integer
;; - score is an integer
;; - over is a bool
;; - pause is a bool

(define-struct world (curr next board rows score over pause))

;; blocks : list
;; the list of possible blocks.

(define blocks
  (list (make-single "red"
                     (list (make-posn 93 13)
                           (make-posn 93 33)
                           (make-posn 93 53)
                           (make-posn 113 53)))
        (make-single "green"
                     (list (make-posn 113 13)
                           (make-posn 113 33)
                           (make-posn 113 53)
                           (make-posn 93 53)))
        (make-single "blue"
                     (list (make-posn 93 13)
                           (make-posn 113 13)
                           (make-posn 73 33)
                           (make-posn 93 33)))
        (make-single "purple"
                     (list (make-posn 73 13)
                           (make-posn 93 13)
                           (make-posn 93 33)
                           (make-posn 113 33)))
        (make-single "lightblue"
                     (list (make-posn 93 13)
                           (make-posn 113 13)
                           (make-posn 93 33)
                           (make-posn 113 33)))
        (make-single "orange"
                     (list (make-posn 93 13)
                           (make-posn 93 33)
                           (make-posn 93 53)
                           (make-posn 93 73)))
        (make-single "magenta"
                     (list (make-posn 93 13)
                           (make-posn 73 33)
                           (make-posn 93 33)
                           (make-posn 113 33)))))

;; init-world : world
;; the initial state of the world

(define init-world (make-world (list-ref blocks (random 7))
                               (list-ref blocks (random 7))
                               empty
                               0 0 #f #f))

;; world/curr : world single -> world
;; changes the current world w by updating curr with a single s
;; (define (world/curr w s)...)

(define (world/curr w s)
  (cond
    [(not (world? w)) (error 'world/curr "world expected")]
    [(not (single? s)) (error 'world/curr "single expected")]
    [else
     (make-world s
                 (world-next w)
                 (world-board w)
                 (world-rows w)
                 (world-score w)
                 (world-over w)
                 (world-pause w))]))

(check-expect (world? (world/curr
                       init-world
                       (make-single "red" (list (make-posn 0 0))))) #t)
(check-error (world/curr 0
                         (make-single "red" (list (make-posn 0 0))))
             "world/curr: world expected")
(check-error (world/curr init-world 0) "world/curr: single expected")

;; world/next : world single -> world
;; changes the current world w by updating next with a single s
;; (define (world/next w s)...)

(define (world/next w s)
  (cond
    [(not (world? w)) (error 'world/next "world expected")]
    [(not (single? s)) (error 'world/next "single expected")]
    [else
     (make-world (world-curr w)
                 s
                 (world-board w)
                 (world-rows w)
                 (world-score w)
                 (world-over w)
                 (world-pause w))]))

(check-expect (world? (world/next
                       init-world
                       (make-single "red" (list (make-posn 0 0))))) #t)
(check-error (world/next 0
                         (make-single "red" (list (make-posn 0 0))))
             "world/next: world expected")
(check-error (world/next init-world 0) "world/next: single expected")

;; world/board : world list -> world
;; changes the current world w by updating board with a list l
;; (define (world/board w l)...)

(define (world/board w l)
  (cond
    [(not (world? w)) (error 'world/board "world expected")]
    [(not (list? l)) (error 'world/board "list expected")]
    [else
     (make-world (world-curr w)
                 (world-next w)
                 l
                 (world-rows w)
                 (world-score w)
                 (world-over w)
                 (world-pause w))]))

(check-expect (world? (world/board
                       init-world
                       (list (make-single "red" (list (make-posn 0 0)))))) #t)
(check-error (world/board 0
                          (list (make-single "red" (list (make-posn 0 0)))))
             "world/board: world expected")
(check-error (world/board init-world 0) "world/board: list expected")

;; world/rows : world number -> world
;; changes the current world w by updating rows with a number n
;; (define (world/rows w n)...)

(define (world/rows w n)
  (cond
    [(not (world? w)) (error 'world/rows "world expected")]
    [(not (number? n)) (error 'world/rows "number expected")]
    [else
     (make-world (world-curr w)
                 (world-next w)
                 (world-board w)
                 n
                 (world-score w)
                 (world-over w)
                 (world-pause w))]))

(check-expect (world? (world/rows init-world 0)) #t)
(check-error (world/rows 0 0) "world/rows: world expected")
(check-error (world/rows init-world "") "world/rows: number expected")

;; world/score : world number -> world
;; changes the current world w by updating score with a number n
;; (define (world/score w n)...)

(define (world/score w n)
  (cond
    [(not (world? w)) (error 'world/score "world expected")]
    [(not (number? n)) (error 'world/score "number expected")]
    [else
     (make-world (world-curr w)
                 (world-next w)
                 (world-board w)
                 (world-rows w)
                 n
                 (world-over w)
                 (world-pause w))]))

(check-expect (world? (world/score init-world 0)) #t)
(check-error (world/score 0 0) "world/score: world expected")
(check-error (world/score init-world "") "world/score: number expected")

;; world/over : world boolean -> world
;; changes the current world w by updating over with a bool b
;; (define (world/over w b)...)

(define (world/over w b)
  (cond
    [(not (world? w)) (error 'world/over "world expected")]
    [(not (boolean? b)) (error 'world/over "boolean expected")]
    [else
     (make-world (world-curr w)
                 (world-next w)
                 (world-board w)
                 (world-rows w)
                 (world-score w)
                 b
                 (world-pause w))]))

(check-expect (world? (world/over init-world #f)) #t)
(check-error (world/over 0 0) "world/over: world expected")
(check-error (world/over init-world "") "world/over: boolean expected")

;; world/pause : world boolean -> world
;; changes the current world w by updating pause with a bool b
;; (define (world/pause w b)...)

(define (world/pause w b)
  (cond
    [(not (world? w)) (error 'world/pause "world expected")]
    [(not (boolean? b)) (error 'world/pause "boolean expected")]
    [else
     (make-world (world-curr w)
                 (world-next w)
                 (world-board w)
                 (world-rows w)
                 (world-score w)
                 (world-over w)
                 b)]))

(check-expect (world? (world/pause init-world #f)) #t)
(check-error (world/pause 0 0) "world/pause: world expected")
(check-error (world/pause init-world "") "world/pause: boolean expected")

;; block : string -> img
;; draws the basic block in color s
;; (define (block s)...)

(define (block s)
  (cond
    [(not (string? s)) (error 'block "string expected")]
    [else (overlay (rectangle 18 18 "solid" s)
                   (rectangle 20 20 "solid" "black"))]))

(check-expect (image? (block "red")) #t)
(check-error (block 0) "block: string expected")

;; background : world -> img
;; draws the background without the blocks
;; (define (background w)...)

(define (background w)
  (cond
    [(not (world? w)) (error 'background "world expected")]
    [else
     (place-image
      (overlay
       (rectangle 200 360 "solid" "white")
       (rectangle 206 366 "solid" "darkblue"))
      103 183
      (place-image
       (overlay
        (rectangle 150 100 "solid" "white")
        (rectangle 156 106 "solid" "darkblue"))
       287 53
       (place-image
        (overlay
         (above
          (overlay/align
           "right" "middle"
           (text/font (number->string (floor (/ (world-rows w) 10)))
                      20 "black" #f `modern `normal `normal #f)
           (overlay/align
            "left" "middle"
            (text/font "level" 20 "black" #f 'modern 'normal 'normal #f)
            (rectangle 150 50 "solid" "white")))
          (rectangle 150 3 "solid" "darkblue")
          (overlay/align
           "right" "middle"
           (text/font (number->string (world-score w))
                      20 "black" #f `modern `normal `normal #f)
           (overlay/align
            "left" "middle"
            (text/font "score" 20 "black" #f 'modern 'normal 'normal #f)
            (rectangle 150 50 "solid" "white")))
          (rectangle 150 3 "solid" "darkblue")
          (overlay/align
           "right" "middle"
           (text/font (number->string (world-rows w))
                      20 "black" #f `modern `normal `normal #f)
           (overlay/align
            "left" "middle"
            (text/font "lines" 20 "black" #f 'modern 'normal 'normal #f)
            (rectangle 150 50 "solid" "white"))))
         (rectangle 156 162 "solid" "darkblue"))
        287 190
        (rectangle 365 366 "solid" "lavender"))))]))

(check-expect (image? (background init-world)) #t)
(check-error (background 0) "background: world expected")


;; render : world -> img
;; takes a world and creates the appropriate image
;; (define (render w)...)

(define (render w)
  (cond
    [(not (world? w)) (error 'render "world expected")]
    [else
     (local
       ((define (draw-board l back)
          (cond
            [(empty? l) back]
            [else (place-image
                   (block (single-color (first l)))
                   (posn-x (first (single-pos (first l))))
                   (posn-y (first (single-pos (first l))))
                   (draw-board (rest l) back))]))
        (define (draw-curr l back)
          (cond
            [(empty? l) back]
            [else (place-image
                   (block (single-color (world-curr w)))
                   (posn-x (first l))
                   (posn-y (first l))
                   (draw-curr (rest l) back))]))
        (define (draw-next l back)
          (cond
            [(empty? l) back]
            [else (place-image
                   (block (single-color (world-next w)))
                   (+ 184 (posn-x (first l)))
                   (+ 10 (posn-y (first l)))
                   (draw-next (rest l) back))])))
       (cond
         [(world-over w)
          (place-image
           (overlay
            (above
             (text/font "Game Over" 35 "black" #f 'modern 'normal 'normal #f)
             (rectangle 10 10 "solid" "white")
             (text/font "Press 'n' for a" 20 "black" #f 'modern 'normal 'normal #f)
             (rectangle 10 10 "solid" "white")
             (text/font "new game." 20 "black" #f 'modern 'normal 'normal #f))
            (rectangle 200 360 "solid" "white"))
           103 183
           (background w))]
         [(world-pause w)
          (place-image
           (overlay
            (above
             (text/font "Game" 35 "black" #f 'modern 'normal 'normal #f)
             (rectangle 10 10 "solid" "white")
             (text/font "Paused" 35 "black" #f 'modern 'normal 'normal #f))
            (rectangle 200 360 "solid" "white"))
           103 183
           (background w))]
         [else
          (draw-curr
           (single-pos (world-curr w))
           (draw-board
            (world-board w)
            (draw-next
             (single-pos (world-next w))
             (background w))))]))]))

(check-expect (image? (render init-world)) #t)
(check-error (render 0) "render: world expected")

;; list-match-y : (list of posns) (list of posns) (num num -> bool) -> bool
;; takes two lists of posns and a boolean function f and determines if any
;;  pair of y coordinates from points with matching x-coordinates satisfy f
;; (define (list-match-x s l f)...)

(define (list-match-y l1 l2 f)
  (cond
    [(or (not (list? l1)) (not (list? l2))) (error 'list-match-y "list expected")]
    [else (cond
            [(empty? l1) #f]
            [(empty? l2) #f]
            [else
             (or
              (foldl
               (lambda (x y) (or x y)) #f
               (map (lambda (x) (f x (posn-y (first l1))))
                    (map
                     (lambda (x) (posn-y x))
                     (filter
                      (lambda (p)
                        (= (posn-x p) (posn-x (first l1)))) l2))))
              (list-match-y (rest l1) l2 f))])]))

(check-expect (boolean? (list-match-y
                         (list (make-posn 0 0))
                         (list (make-posn 0 0))
                         (lambda (x y) (< x y)))) #t)
(check-error (list-match-y 0 0 0) "list-match-y: list expected")

;; list-match-x : (list of posns) (num -> bool) -> bool
;; takes a lists of posns and and a function and finds if the length of any
;; list of x coordiantes with matching y coordinates satisfy f
;; (define (list-match-x l f)...)

(define (list-match-x l f)
  (cond
    [(not (list? l)) (error 'list-match-x "list expected")]
    [else (cond
            [(empty? l) #f]
            [else (foldl (lambda (x y) (or x y)) #f
                         (map
                          (lambda (y)
                            (f
                             (length
                              (filter
                               (lambda (x) (= (posn-y x) (posn-y y))) l))))
                          l))])]))

(check-expect (boolean? (list-match-x
                         (list (make-posn 0 0))
                         (lambda (x) (= x 10)))) #t)
(check-error (list-match-x 0 0) "list-match-x: list expected")

;; list-find-x : (list of singles) (num -> bool) -> list
;; just like list-match-x, but finds all such lists and makes a list out of them
;; (define (list-find-x l f)...)

(define (list-find-x l f)
  (cond
    [(not (list? l)) (error 'list-find-x "list expected")]
    [else (cond
            [(empty? l) 0]
            [else (map
                   (lambda (y)
                     (cond
                       [(f
                         (length
                          (filter
                           (lambda (x) (=
                                        (posn-y (first (single-pos x)))
                                        (posn-y (first (single-pos y))))) l)))
                        (filter
                         (lambda (x) (=
                                      (posn-y (first (single-pos x)))
                                      (posn-y (first (single-pos y))))) l)]
                       [else empty]))
                   l)])]))

(check-expect (list? (list-find-x
                      (list (make-single "red"
                                         (list (make-posn 0 0))))
                      (lambda (x) (= x 10)))) #t)
(check-error (list-find-x 0 0) "list-find-x: list expected")

;; delete-list : list list -> list
;; takes two lists and removes the first instance of each element
;;  in l1 from l2
;; (delete-list (l1 l2)...)

(define (delete-list l1 l2)
  (cond
    [(or (not (list? l1)) (not (list? l2))) (error 'delete-list "list expected")]
    [else (cond
            [(empty? l1) l2]
            [(empty? l2) empty]
            [else (delete-list (rest l1) (remove (first l1) l2))])]))

(check-expect (list? (delete-list empty empty)) #t)
(check-expect (delete-list '(1 2 3) '(1 2 3 4 5 6)) '(4 5 6))
(check-error (delete-list 0 empty) "delete-list: list expected")
(check-error (delete-list empty 0) "delete-list: list expected")

;; shift-blocks : (list of singles) (list of singles) num -> (list of singles)
;; takes two lists of singles with singleton position lists, finds all the
;;  singles in the second list with matching x coordinates to those in the
;;  first list and shifts them down by num if they have a smaller y coordiante
;; (define (shift-blocks l1 l2 n)...)

(define (shift-blocks l1 l2 n)
  (cond
    [(or (not (list? l1)) (not (list? l2))) (error 'shift-blocks "list expected")]
    [(not (number? n)) (error 'shift-blocks "number expected")]
    [(or (empty? l1) (empty? l2)) l2]
    [else
     (map
      (lambda (x)
        (make-single
         (single-color x)
         (list
          (make-posn
           (posn-x
            (first
             (single-pos x)))
           (cond
             [(and
               (member? (posn-x
                         (first
                          (single-pos x)))
                        (map
                         (lambda (y) (posn-x
                                      (first
                                       (single-pos y)))) l1))
               (< (posn-y
                   (first
                    (single-pos x)))
                  (foldl max 0
                         (map
                          (lambda (y) (posn-y
                                       (first
                                        (single-pos y)))) l1))))
              (+ n (posn-y
                    (first
                     (single-pos x))))]
             [else (posn-y
                    (first
                     (single-pos x)))]))))) l2)]))

(check-expect (list? (shift-blocks empty empty 0)) #t)
(check-error (shift-blocks 0 empty 0) "shift-blocks: list expected")
(check-error (shift-blocks empty 0 0) "shift-blocks: list expected")
(check-error (shift-blocks empty empty "") "shift-blocks: number expected")

;; move-block : world num -> world
;; moves a block in world w down at speed s
;; (define (move-block w s)...)

(define (move-block w s)
  (cond
    [(not (world? w)) (error 'move-block "world expected")]
    [(not (number? s)) (error 'move-block "number expected")]
    [else
     (cond
       ;; block is too high
       [(and
         (list-match-y (single-pos (world-curr w))
                       (map (lambda (x) (first (single-pos x))) (world-board w))
                       (lambda (x y) (<= (- x y) 20)))
         (foldl (lambda (x y) (or x y)) #f
                (map (lambda (x) (>= 20 (posn-y x)))
                     (single-pos (world-curr w)))))
        (world/over w #t)]
       ;; block moves outside of left edge
       [(foldl (lambda (x y) (or x y)) #f
               (map (lambda (x) (> 13 (posn-x x)))
                    (single-pos (world-curr w))))
        (world/curr
         w
         (make-single
          (single-color (world-curr w))
          (map
           (lambda
               (x)
             (make-posn
              (+
               (- 13 (foldl min 13
                            (map (lambda (y) (posn-x y))
                                 (single-pos (world-curr w)))))
               (posn-x x))
              (posn-y x)))
           (single-pos (world-curr w)))))]
       ;; block moves outside of right edge
       [(foldl (lambda (x y) (or x y)) #f
               (map (lambda (x) (< 193 (posn-x x)))
                    (single-pos (world-curr w))))
        (world/curr
         w
         (make-single
          (single-color (world-curr w))
          (map
           (lambda
               (x)
             (make-posn
              (+
               (- 193 (foldl max 193
                             (map (lambda (y) (posn-x y))
                                  (single-pos (world-curr w)))))
               (posn-x x))
              (posn-y x)))
           (single-pos (world-curr w)))))]
       ;; block hits blocks on bottom
       [(list-match-y (single-pos (world-curr w))
                      (map (lambda (x) (first (single-pos x))) (world-board w))
                      (lambda (x y) (<= (abs (- x y)) 20)))
        (world/next
         (world/curr
          (world/board
           w
           (append
            (map (lambda (x)
                   (make-single (single-color (world-curr w)) (list x)))
                 (local
                   ((define (check x y f)
                      (cond
                        [(not (list-match-y x y f)) x]
                        [else
                         (check
                          (map
                           (lambda (x)
                             (make-posn
                              (posn-x x)
                              (+ 13
                                 (* 20
                                    (floor
                                     (/
                                      (-
                                       (posn-y x) 3) 20)))))) x)
                          y f)])))
                   (check (single-pos (world-curr w))
                          (map
                           (lambda (x) (first (single-pos x)))
                           (world-board w))
                          (lambda (x y) (<= (abs (- x y)) 19)))))
            (world-board w)))
          (world-next w))
         (list-ref blocks (random 7)))]
       ;; block hits bottom
       [(foldl (lambda (x y) (or x y)) #f
               (map (lambda (x) (<= 353 (posn-y x)))
                    (single-pos (world-curr w))))
        (world/next
         (world/curr
          (world/board
           w
           (append
            (map
             (lambda (x)
               (make-single
                (single-color (world-curr w))
                (list (make-posn
                       (posn-x x)
                       (- (posn-y x)
                          (- (foldl max 0
                                    (map (lambda (x) (posn-y x))
                                         (single-pos (world-curr w))))
                             353))))))
             (single-pos (world-curr w)))
            (world-board w)))
          (world-next w))
         (list-ref blocks (random 7)))]
       ;; block falls normally at speed s
       [else (world/curr
              w
              (make-single
               (single-color (world-curr w))
               (map (lambda (x) (make-posn (posn-x x)
                                           (+ s (posn-y x))))
                    (single-pos (world-curr w)))))])]))

(check-expect (world? (move-block init-world 1)) #t)
(check-error (move-block 0 0) "move-block: world expected")
(check-error (move-block init-world "") "move-block: number expected")

;; left-right : world num -> world
;; moves the current block left or right n pixels
;; (define (left-right w n)...)

(define (left-right w n)
  (cond
    [(not (world? w)) (error 'left-right "world expected")]
    [(not (number? n)) (error 'left-right "number expected")]
    [else (cond
            [(or 
              ;; block hits left side
              (and (foldl (lambda (x y) (or x y)) #f
                          (map (lambda (x) (>= 13 (posn-x x)))
                               (single-pos (world-curr w))))
                   (< n 0))
              ;; block hits right side
              (and (foldl (lambda (x y) (or x y)) #f
                          (map (lambda (x) (<= 193 (posn-x x)))
                               (single-pos (world-curr w))))
                   (> n 0))
              ;; block hits blocks on the left
              (and
               (list-match-y
                (single-pos (world-curr w))
                (map (lambda (x) (make-posn
                                  (+ 20 (posn-x (first (single-pos x))))
                                  (posn-y (first (single-pos x)))))
                     (world-board w))
                (lambda (x y) (>= 19 (abs (- x y)))))
               (< n 0))
              ;; block hits blocks on the right   
              (and
               (list-match-y
                (single-pos (world-curr w))
                (map (lambda (x) (make-posn
                                  (- (posn-x (first (single-pos x))) 20)
                                  (posn-y (first (single-pos x)))))
                     (world-board w))
                (lambda (x y) (>= 19 (abs (- x y)))))
               (> n 0)))
             w]
            [else (world/curr
                   w
                   (make-single
                    (single-color (world-curr w))
                    (map (lambda (x) (make-posn (+ n (posn-x x))
                                                (posn-y x)))
                         (single-pos (world-curr w)))))])]))

(check-expect (world? (left-right init-world 0)) #t)
(check-error (left-right 0 0) "left-right: world expected")
(check-error (left-right init-world "") "left-right: number expected")

;; rotate-block : world -> world
;; rotates the current block π/2 counterclockwise
;; (define (rotate-block w)...)

(define (rotate-block w)
  (cond
    [(not (world? w)) (error 'rotate-block "world expected")]
    [else
     (local
       ((define
          ;; defines the centroid of the shape
          center (make-posn
                  (/ (foldl + 0 (map (lambda (x) (posn-x x))
                                     (single-pos (world-curr w)))) 4)
                  (/ (foldl + 0 (map (lambda (x) (posn-y x))
                                     (single-pos (world-curr w)))) 4)))
        (define (rotate-list l)
          (map
           ;; moves the centroid to the origin, rotates and moves back
           (lambda
               (x)
             (make-posn
              (- (* 20
                    (round
                     (/ (+ (- (posn-y center) (posn-y x)) (posn-x center))
                        20))) 7)
              (+ (- (posn-x x) (posn-x center)) (posn-y center)))) l))
        (define rotated-list (rotate-list (single-pos (world-curr w)))))
       (world/curr
        w
        (make-single
         (single-color (world-curr w))
         (cond
           ;; checks to make sure the rotated pieces don't hit the board pieces
           [(list-match-y rotated-list
                          (map
                           (lambda (x) (first (single-pos x)))
                           (world-board w))
                          (lambda (x y) (<= (abs (- x y)) 19)))
            (single-pos (world-curr w))]
           [else rotated-list]))))]))

(check-expect (world? (rotate-block init-world)) #t)
(check-error (rotate-block 0) "rotate-block: world expected")

;; tick : world -> world
;; determines the state of the world at each time interval
;; (define (tick w)...)

(define (tick w)
  (cond
    [(not (world? w)) (error 'tick "world expected")]
    ;; check to see if a row should be cleared
    [(list-match-x (map
                    (lambda (x) (first (single-pos x)))
                    (world-board w))
                   (lambda (x) (= x 10)))
     (local ((define remove-list
               (foldl append empty
                      (list-find-x (world-board w)
                                   (lambda (x) (= x 10)))))
             (define rows
               (/ (length remove-list) 100)))
       (world/score
        (world/rows
         (world/board
          w
          (shift-blocks 
           remove-list
           (delete-list
            remove-list
            (world-board w))
           (* 20 rows)))
         (+ rows (world-rows w)))
        (+ (* 100 rows)
           (world-score w))))]
    ;; checks if the world is paused
    [(world-pause w) w]
    ;; moves block at speed determined by the number of rows cleared
    [else (move-block w (+ 1 (floor (/ (world-rows w) 10))))]))

;; read-key : world key -> world
;; reads key input and changes the world accordingly
;; (define (read-key w k)...)

(define (read-key w k)
  (cond
    [(not (world? w)) (error 'read-key "world expected")]
    [else (cond
            [(and (world-pause w)
                  (not (key=? k "p"))) w]
            [else (cond
                    [(key=? k "left")
                     (left-right w -20)]
                    [(key=? k "right")
                     (left-right w 20)]
                    [(key=? k "down")
                     (move-block w 10)]
                    [(key=? k "up")
                     (rotate-block w)]
                    [(key=? k "n")
                     (cond
                       [(world-over w) init-world]
                       [else w])]
                    [(key=? k "p")
                     (cond
                       [(world-pause w) (world/pause w #f)]
                       [else (world/pause w #t)])]
                    [else w])])]))

(big-bang init-world (on-tick tick) (to-draw render) (on-key read-key))