;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname Pong) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;              Pong              ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Kris Harper
;; 10.25.10

;; a basic pong game
;; use w and s to move the left paddle and up and down to move the right paddle
;; use = and - to make the ball move faster or slower
;; use 1 and 2 to switch between one and two players

(require 2htdp/image)
(require 2htdp/universe)
(require htdp/matrix)
(require 2htdp/batch-io)

;; a world is a (make-world ball last left right lscore rscore speed players), where
;; - ball is a posn
;; - last is a posn
;; - left is a posn
;; - right is a posn
;; - lscore is an integer
;; - rscore is an integer
;; - speed is a number
;; - players is an integer

(define-struct world (ball last left right lscore rscore speed players))

;; some defined dimension constants

(define board-w 600)
(define board-h 400)
(define pad-h 60)
(define pad-speed 10)
(define pad-distance (sqrt (+ (* (/ pad-h 2) (/ pad-h 2)) 25)))
(define ball-speed 5)
(define comp-speed 7)
(define init-world (local ((define ball1 (+ 30 (random 70)))
                           (define ball2 (+ 30 (random (- board-h 60)))))
                     (make-world
                      (make-posn ball1 ball2)
                      (make-posn
                       (- ball1 (+ (random 6) 1))
                       (+ ball2 (- (random 11) 5)))
                      (make-posn 5 (/ board-h 2))
                      (make-posn (- board-w 5) (/ board-h 2))
                      0 0 5 2)))

;; On the advice of Adam Shaw, I added a sequence of functions which
;;  change one value of world and leave the others the same. This is
;;  really useful for a case-by-case animation like this when each case
;;  changes one aspect of the world.

;; world/ball : world posn -> world
;; takes a world and gives back the same world with p for world-ball
;; (define (world/ball w p)...)

(define (world/ball w p)
  (cond
    [(not (world? w)) (error 'world/ball "world expected")]
    [(not (posn? p)) (error 'world/ball "posn expected")]
    [else (make-world p
                      (world-last w)
                      (world-left w)
                      (world-right w)
                      (world-lscore w)
                      (world-rscore w)
                      (world-speed w)
                      (world-players w))]))

(check-expect (world? (world/ball init-world (make-posn 0 0))) #t)
(check-error (world/ball 0 (make-posn 0 0)) "world/ball: world expected")
(check-error (world/ball init-world 0) "world/ball: posn expected")

;; world/last : world posn -> world
;; takes a world and gives back the same world with p for world-last
;; (define (world/last w p)...)

(define (world/last w p)
  (cond
    [(not (world? w)) (error 'world/last "world expected")]
    [(not (posn? p)) (error 'world/last "posn expected")]
    [else (make-world (world-ball w)
                      p
                      (world-left w)
                      (world-right w)
                      (world-lscore w)
                      (world-rscore w)
                      (world-speed w)
                      (world-players w))]))

(check-expect (world? (world/last init-world (make-posn 0 0))) #t)
(check-error (world/last 0 (make-posn 0 0)) "world/last: world expected")
(check-error (world/last init-world 0) "world/last: posn expected")

;; world/left : world posn -> world
;; takes a world and gives back the same world with p for world-left
;; (define (world/left w p)...)

(define (world/left w p)
  (cond
    [(not (world? w)) (error 'world/left "world expected")]
    [(not (posn? p)) (error 'world/left "posn expected")]
    [else (make-world (world-ball w)
                      (world-last w)
                      p
                      (world-right w)
                      (world-lscore w)
                      (world-rscore w)
                      (world-speed w)
                      (world-players w))]))

(check-expect (world? (world/left init-world (make-posn 0 0))) #t)
(check-error (world/left 0 (make-posn 0 0)) "world/left: world expected")
(check-error (world/left init-world 0) "world/left: posn expected")

;; world/right : world posn -> world
;; takes a world and gives back the same world with p for world-right
;; (define (world/right w p)...)

(define (world/right w p)
  (cond
    [(not (world? w)) (error 'world/right "world expected")]
    [(not (posn? p)) (error 'world/right "posn expected")]
    [else (make-world (world-ball w)
                      (world-last w)
                      (world-left w)
                      p
                      (world-lscore w)
                      (world-rscore w)
                      (world-speed w)
                      (world-players w))]))

(check-expect (world? (world/right init-world (make-posn 0 0))) #t)
(check-error (world/right 0 (make-posn 0 0)) "world/right: world expected")
(check-error (world/right init-world 0) "world/right: posn expected")

;; world/lscore : world num -> world
;; takes a world and gives back the same world with n for world-lscore
;; (define (world/lscore w n)...)

(define (world/lscore w n)
  (cond
    [(not (world? w)) (error 'world/lscore "world expected")]
    [(not (number? n)) (error 'world/lscore "number expected")]
    [else (make-world (world-ball w)
                      (world-last w)
                      (world-left w)
                      (world-right w)
                      n
                      (world-rscore w)
                      (world-speed w)
                      (world-players w))]))

(check-expect (world? (world/lscore init-world 0)) #t)
(check-error (world/lscore 0 0) "world/lscore: world expected")
(check-error (world/lscore init-world "") "world/lscore: number expected")

;; world/rscore : world num -> world
;; takes a world and gives back the same world with n for world-rscore
;; (define (world/rscore w n)...)

(define (world/rscore w n)
  (cond
    [(not (world? w)) (error 'world/rscore "world expected")]
    [(not (number? n)) (error 'world/rscore "number expected")]
    [else (make-world (world-ball w)
                      (world-last w)
                      (world-left w)
                      (world-right w)
                      (world-lscore w)
                      n
                      (world-speed w)
                      (world-players w))]))

(check-expect (world? (world/rscore init-world 0)) #t)
(check-error (world/rscore 0 0) "world/rscore: world expected")
(check-error (world/rscore init-world "") "world/rscore: number expected")

;; world/speed : world num -> world
;; takes a world and gives back the same world with n for world-speed
;; (define (world/speed w n)...)

(define (world/speed w n)
  (cond
    [(not (world? w)) (error 'world/speed "world expected")]
    [(not (number? n)) (error 'world/speed "number expected")]
    [else (make-world (world-ball w)
                      (world-last w)
                      (world-left w)
                      (world-right w)
                      (world-lscore w)
                      (world-rscore w)
                      n
                      (world-players w))]))

(check-expect (world? (world/speed init-world 0)) #t)
(check-error (world/speed 0 0) "world/speed: world expected")
(check-error (world/speed init-world "") "world/speed: number expected")

;; world/players : world num -> world
;; takes a world and gives back the same world with n for world-players
;; (define (world/players w n)...)

(define (world/players w n)
  (cond
    [(not (world? w)) (error 'world/players "world expected")]
    [(not (number? n)) (error 'world/players "number expected")]
    [else (make-world (world-ball w)
                      (world-last w)
                      (world-left w)
                      (world-right w)
                      (world-lscore w)
                      (world-rscore w)
                      (world-speed w)
                      n)]))

(check-expect (world? (world/players init-world 0)) #t)
(check-error (world/players 0 0) "world/players: world expected")
(check-error (world/players init-world "") "world/players: number expected")

;; distance : posn posn -> num
;; returns the distance between p1 and p2
;; (define (distance p1 p2)...)

(define (distance p1 p2)
  (cond
    [(not (and (posn? p1) (posn? p2))) (error 'distance "posn expected")]
    [else (sqrt
           (+
            (* (- (posn-x p1) (posn-x p2)) (- (posn-x p1) (posn-x p2)))
            (* (- (posn-y p1) (posn-y p2)) (- (posn-y p1) (posn-y p2)))))]))

(check-expect (distance (make-posn 0 0) (make-posn 4 3)) 5)
(check-expect (distance (make-posn 1 2) (make-posn -2 -2)) 5)
(check-error (distance 0 0) "distance: posn expected")

;; render : world -> scene
;; renders a scene based on a given world
;; (define (render w)...)

(define (render w)
  (cond
    [(not (world? w)) (error 'render "world expected")]
    [else (place-image
           (rectangle 10 pad-h "solid" "white")
           (posn-x (world-left w))
           (posn-y (world-left w))
           (place-image
            (rectangle 10 pad-h "solid" "white")
            (posn-x (world-right w))
            (posn-y (world-right w))
            (place-image
             (circle 10 "solid" "white")
             (posn-x (world-ball w))
             (posn-y (world-ball w))
             (place-image
              (text (number->string (world-lscore w)) 20 "white")
              25 15
              (place-image
               (text (number->string (world-rscore w)) 20 "white")
               (- board-w 25) 15
               (place-image
                (rectangle 5 board-h "solid" "white")
                (/ board-w 2) (/ board-h 2)
                (rectangle board-w board-h "solid" "black")))))))]))

(check-expect (image? (render init-world)) #t)
(check-error (render 0) "render: world expected")

;; move-ball : posn posn num -> posn
;; calculates the next position of the ball based on current
;;  and last positions and the speed
;; (define (move-ball p1 p2 s)...)

(define (move-ball p1 p2 s)
  (cond
    [(or (not (posn? p1)) (not (posn? p2))) (error 'move-ball "posn expected")]
    [(not (number? s)) (error 'move-ball "number expected")]
    [else (local ((define x1 (posn-x p1))
                  (define x2 (posn-x p2))
                  (define y1 (posn-y p1))
                  (define y2 (posn-y p2))
                  (define norm (sqrt (+ (* (- x2 x1)
                                           (- x2 x1))
                                        (* (- y2 y1)
                                           (- y2 y1)))))
                  (define x3 (+ x2 (* s (- x2 x1) (/ 1 norm))))
                  (define y3 (+ y2 (* s (- y2 y1) (/ 1 norm)))))
            (make-posn x3 y3))]))

(check-expect (posn? (move-ball
                      (make-posn 100 100)
                      (make-posn 101 100) 3)) #t)
(check-expect (move-ball
               (make-posn 100 100)
               (make-posn 101 100) 4) (make-posn (+ 101 4) 100))
(check-error (move-ball 0 0 0) "move-ball: posn expected")
(check-error (move-ball
              (make-posn 100 100)
              (make-posn 101 100) "") "move-ball: number expected")

;; move-pad : posn string num -> posn
;; gives the new posn for a moved paddle using "up" or "down" and a speed
;; (define (move-pad p str s)...)

(define (move-pad p str s)
  (cond
    [(not (posn? p)) (error 'move-pad "posn expected")]
    [(not (string? str)) (error 'move-pad "string expected")]
    [(not (number? s)) (error 'move-pad "number expected")]
    [else (make-posn
           (posn-x p)
           (cond
             [(string=? str "up")
              (cond
                [(< 0 (- (posn-y p) (/ pad-h 2)))
                 (- (posn-y p) s)]
                [else (posn-y p)])]
             [(string=? str "down")
              (cond
                [(> board-h (+ (posn-y p) (/ pad-h 2)))
                 (+ (posn-y p) s)]
                [else (posn-y p)])]))]))

(check-expect (posn? (move-pad (make-posn 5 100) "up" 2)) #t)
(check-expect (move-pad (make-posn 5 100) "up" 2) (make-posn 5 (- 100 2)))
(check-expect (move-pad (make-posn 5 100) "down" 2) (make-posn 5 (+ 100 2)))
(check-error (move-pad 0 "up" 2) "move-pad: posn expected")
(check-error (move-pad (make-posn 5 100) 0 2) "move-pad: string expected")
(check-error (move-pad (make-posn 5 100) "up" "") "move-pad: number expected")

;; prop : posn posn -> num
;; determine where the y coordinate of the ball is in relation to the paddles.
;;  if num < 0 or num > 1 then the ball is above or below the paddle
;;  if 0 <= num <= 1 then the ball is between the top and bottom
;;  of the paddle and num determines the proportion.
;; (define (prop p1 p2)...)

(define (prop p1 p2)
  (cond
    [(or (not (posn? p1)) (not (posn? p2))) (error 'prop "posn expected")]
    [else (/ (- (+ (posn-y p1)
                   (/ pad-h 2))
                (posn-y p2))
             pad-h)]))

(check-expect (number? (prop (make-posn 0 0) (make-posn 0 0))) #t)
(check-error (prop 0 0) "prop: posn expected")

;; pad-hit : posn posn posn -> bool
;; decides if the ball has hit a paddle
;; (define (pad-hit p1 p2 p3)...)

(define (pad-hit p1 p2 p3)
  (cond
    [(or (not (posn? p1)) (not (posn? p2)) (not (posn? p3)))
     (error 'pad-hit "posn expected")]
    [else (cond
            [(and
              (and (and
                    (<= 0 (prop p1 p2))
                    (<= (prop p1 p2) 1))
                   (>= 15 (abs (- (posn-x p1) (posn-x p2)))))
              (or (and
                   (= (posn-x p1) 5)
                   (> 0 (- (posn-x p2) (posn-x p3))))
                  (and
                   (= (posn-x p1) (- board-w 5))
                   (< 0 (- (posn-x p2) (posn-x p3)))))) #t]
            [else #f])]))

(check-expect (boolean? (pad-hit
                         (make-posn 0 0)
                         (make-posn 0 0)
                         (make-posn 0 0))) #t)
(check-error (pad-hit 0 0 0) "pad-hit: posn expected")

;; corner : posn posn posn -> bool
;; decides if the ball is hitting the corner of the paddle
;; (define (corner p1 p2 p3)...)

(define (corner p1 p2 p3)
  (cond
    [(or
      (not (posn? p1))
      (not (posn? p2))
      (not (posn? p3))) (error 'corner "posn expected")]
    [else (cond
            [(and
              (and (or
                    (>= 0 (prop p1 p2))
                    (>= (prop p1 p2) 1))
                   (or
                    (>= 10 (distance
                            p2
                            (make-posn
                             (posn-x p1)
                             (+ (posn-y p1) (/ pad-h 2)))))
                    (>= 10 (distance
                            p2
                            (make-posn
                             (posn-x p1)
                             (- (posn-y p1) (/ pad-h 2)))))))
              (or (and
                   (= (posn-x p1) 5)
                   (> 0 (- (posn-x p2) (posn-x p3))))
                  (and
                   (= (posn-x p1) (- board-w 5))
                   (< 0 (- (posn-x p2) (posn-x p3)))))) #t]
            [else #f])]))

(check-expect (boolean? (corner
                         (make-posn 0 0)
                         (make-posn 0 0)
                         (make-posn 0 0))) #t)
(check-error (corner 0 0 0) "corner: posn expected")

;; find-angle : posn posn -> num
;; determines the angle the ball should be rebounded off the paddle
;; (define (find-angle p1 p2)...)

(define (find-angle p1 p2)
  (cond
    [(or (not (posn? p1)) (not (posn? p2))) (error 'find-angle "posn expected")]
    [else (local ((define (f x)
                    (+ (* (/ 6.3 84)
                          (* x x))
                       (* (/ -6.3 12) x)
                       2.4)))
            (f (floor (* 8 (prop p1 p2)))))]))

(check-expect (number? (find-angle (make-posn 0 0) (make-posn 0 0))) #t)
(check-error (find-angle 0 0) "find-angle: posn expected")

;; tick : world -> world
;; changes the world step by step
;; (define (tick w)...)

(define (tick w)
  (cond
    [(not (world? w)) (error 'tick "world expected")]
    [else (cond
            ;; ball hits the top or bottom
            [(or (>= (posn-y (world-ball w)) (- board-h 10))
                 (<= (posn-y (world-ball w)) 10))
             (world/last
              (world/ball
               w
               (move-ball
                (make-posn
                 (posn-x (world-last w))
                 (- (* 2 (posn-y (world-ball w)))
                    (posn-y (world-last w))))
                (world-ball w)
                (world-speed w)))
              (world-ball w))]
            ;; ball hits a corner of a paddle
            [(or (corner (world-left w) (world-ball w) (world-last w))
                 (corner (world-right w) (world-ball w) (world-last w)))
             (world/last
              (world/ball
               w
               (move-ball
                (make-posn
                 (- (* 2 (posn-x (world-ball w)))
                    (posn-x (world-last w)))
                 (posn-y (world-last w)))
                (world-ball w)
                (world-speed w)))
              (world-ball w))]
            ;; ball hits a paddle
            [(or (pad-hit (world-left w) (world-ball w) (world-last w))
                 (pad-hit (world-right w) (world-ball w) (world-last w)))
             (world/last
              (world/ball
               w
               (move-ball
                (make-posn
                 (- (* 2 (posn-x (world-ball w)))
                    (posn-x (world-last w)))
                 (cond
                   [(> .1 (abs
                           (- (posn-y (world-ball w))
                              (posn-y (world-last w)))))
                    (+ (- 300 (random 601)) (posn-y (world-last w)))]
                   [else (posn-y (world-last w))]))
                (world-ball w)
                (world-speed w)))
              (world-ball w))]
            ;; ball misses paddle and hits left wall
            [(< (posn-x (world-ball w)) 10)
             (local ((define ball1 (+ 30 (random 70)))
                     (define ball2 (+ 30 (random (- board-h 60)))))
               (world/rscore
                (world/last
                 (world/ball
                  w
                  (make-posn ball1 ball2))
                 (make-posn
                  (- ball1 (+ (random 6) 1))
                  (+ ball2 (- (random 11) 5))))
                (+ 1 (world-rscore w))))]
            ;; ball misses paddle and hits right wall
            [(< (- board-w (posn-x (world-ball w))) 10)
             (local ((define ball1 (- board-w (+ 30 (random 70))))
                     (define ball2 (+ 30 (random (- board-h 60)))))
               (world/lscore
                (world/last
                 (world/ball
                  w
                  (make-posn ball1 ball2))
                 (make-posn
                  (+ ball1 (+ (random 6) 1))
                  (+ ball2 (- (random 11) 5))))
                (+ 1(world-lscore w))))]
            ;; ball is in normal play with one player
            [(= (world-players w) 1)
             (cond
               [(and
                 (>= (- (posn-y (world-right w)) (/ pad-h 2))
                     (posn-y (world-ball w)))
                 (> (posn-x (world-ball w)) (/ board-w 2))
                 (< 0 (- (posn-x (world-ball w)) (posn-x (world-last w)))))
                (world/ball
                 (world/right
                  w
                  (move-pad (world-right w) "up" comp-speed))
                 (move-ball
                  (world-last w)
                  (world-ball w)
                  (world-speed w)))]
               [(and
                 (<= (+ (posn-y (world-right w)) (/ pad-h 2))
                     (posn-y (world-ball w)))
                 (> (posn-x (world-ball w)) (/ board-w 2))
                 (< 0 (- (posn-x (world-ball w)) (posn-x (world-last w)))))
                (world/ball
                 (world/right
                  w
                  (move-pad (world-right w) "down" comp-speed))
                 (move-ball
                  (world-last w)
                  (world-ball w)
                  (world-speed w)))]
               [else (world/ball
                      w
                      (move-ball
                       (world-last w)
                       (world-ball w)
                       (world-speed w)))])]
            ;; ball is in normal play with two players
            [else
             (world/ball
              w
              (move-ball
               (world-last w)
               (world-ball w)
               (world-speed w)))])]))

(check-expect (world? (tick init-world)) #t)
(check-error (tick 0) "tick: world expected")

;; read-key : world key -> world
;; takes key inputs and changes the world accordingly
;; (define (read-key w k)...)

(define (read-key w k)
  (cond
    [(not (world? w)) (error 'read-key "world expected")]
    [else (cond
            [(key=? k "w")
             (world/left w (move-pad (world-left w) "up" pad-speed))]
            [(key=? k "s")
             (world/left w (move-pad (world-left w) "down" pad-speed))]
            [(key=? k "up")
             (cond
               [(= (world-players w) 2)
                (world/right w (move-pad (world-right w) "up" pad-speed))]
               [else w])]
            [(key=? k "down")
             (cond
               [(= (world-players w) 2)
                (world/right w (move-pad (world-right w) "down" pad-speed))]
               [else w])]
            [(key=? k "=")
             (world/speed w (+ (world-speed w) 1))]
            [(key=? k "-")
             (cond
               [(< 1 (world-speed w))
                (world/speed w (- (world-speed w) 1))]
               [else w])]
            [(key=? k "1")
             (world/players w 1)]
            [(key=? k "2")
             (world/players w 2)]
            [else w])]))

(check-expect (world? (read-key init-world "w")) #t)
(check-expect (world? (read-key init-world "s")) #t)
(check-expect (world? (read-key init-world "up")) #t)
(check-expect (world? (read-key init-world "down")) #t)
(check-expect (world? (read-key init-world "-")) #t)
(check-expect (world? (read-key init-world "=")) #t)
(check-expect (world? (read-key init-world "1")) #t)
(check-expect (world? (read-key init-world "2")) #t)
(check-error (read-key 0 0) "read-key: world expected")

(big-bang init-world (on-tick tick) (to-draw render) (on-key read-key))