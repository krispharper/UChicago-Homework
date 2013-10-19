;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname |2|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Kris Harper
;; Lab 2 
;; CMSC 15100
;; 10.10.10

(require 2htdp/image)
(require 2htdp/universe)

;; named constants

(define bg-width 300)
(define bg-height 300)
(define init-sun-size 20)
(define wing-angle 30)
(define wing-height 180)

;; data definitions

;; a pos is a (make-pos x y), where
;; - x is a number, and
;; - y is a number
(define-struct pos (x y))  

;; a cloud is a (make-cloud size pos), where
;; - size is a number, and
;; - pos is a pos
(define-struct cloud (size pos))

;; a sun is a (make-sun size pos), where
;; - size is a number, and
;; - pos is a pos
(define-struct sun (size pos))

;; a world is a (make-world c1 c2 c3 sun), where
;; - c1, c2 and c3 are clouds, and
;; - sun is a sun.
;; c1 is the closest cloud, then c2, then c3.
(define-struct world (c1 c2 c3 sun))

;; initial state of the world

;; init-cloud1 : cloud
(define init-cloud1
  (make-cloud (* bg-width (/ 13 20))
              (make-pos (* bg-width (/ 5 4))
                        (* bg-height (/ 4 5)))))

;; init-cloud2 : cloud
(define init-cloud2
  (make-cloud (* bg-width (/ 2 5))
              (make-pos (* bg-width (/ 9 10))
                        (* bg-height (/ 7 10)))))

;; init-cloud3 : cloud
(define init-cloud3
  (make-cloud (* bg-width (/ 1 4))
              (make-pos (* bg-width (/ 7 10))
                        (* bg-height (/ 3 5)))))
;; init-sun : sun
(define init-sun
  (make-sun init-sun-size
            (make-pos (* bg-width (/ 3 20))
                      (* bg-height (/ 3 20)))))

;; init-world : world
(define init-world
  (make-world init-cloud1 init-cloud2 init-cloud3 init-sun))

;; wing-pic : image
(define wing-pic
  (crop 0 
        (/ wing-height 3) 
        wing-height 
        wing-height 
        (isosceles-triangle wing-height 
                            wing-angle 
                            "solid" 
                            "silver")))

;; cloud-pic : num -> img
;; consume a height (in pixels) and draw a "cloud" (crudely).
(define (cloud-pic height)
  (overlay (ellipse (+ height 1) (+ (/ height 5) 1) "outline" "gray")
           (ellipse height (/ height 5) "solid" "ivory")))

;; background : num num -> scene
;; consume a width and height and produce a background scene of
;;   the sky and Lake Michigan.
(define (background width height)
  (place-image 
   (above (rectangle width height "solid" "dodgerblue") 
          (rectangle width (/ height 3) "solid" "teal"))
   (/ width 2) 
   (/ height 2)
   (empty-scene width height)))

;; == write your code below ==

;; Part 1

;; place-sun : sun scene -> scene
;; draws a given sun in a given scene
;; (define (place-sun sun scene)...)

(define (place-sun sun scene)
  (place-image (circle (sun-size sun) "solid" "yellow")
               (pos-x (sun-pos sun))
               (pos-y (sun-pos sun))
               scene))

;; place-cloud : cloud scene -> scene
;; drraws a given cloud in a given scene
;; (define (place-cloud cloud scene)...)

(define (place-cloud cloud scene)
  (place-image (cloud-pic (cloud-size cloud))
               (pos-x (cloud-pos cloud))
               (pos-y (cloud-pos cloud))
               scene))

;; overlay-wing : scene -> scene
;; draws a wing on a given scene
;; (define (overlay-wing scene)...)

(define (overlay-wing scene)
  (place-image wing-pic
               (/ (image-width scene) 2)
               (image-height scene)
               scene))

;; render : world -> scene
;; takes a world and draws a scene with the required components
;; (define (render w)...)

(define (render w)
  (overlay-wing
   (place-cloud (world-c1 w)
                (place-cloud (world-c2 w)
                             (place-cloud (world-c3 w)
                                          (place-sun (world-sun w)
                                                     (background bg-width bg-height)))))))

;; (render init-world)

;; Part 2

;; move-x : num pos -> pos
;; adds a number num to the x-coordinate of pos
;; (define (move-x n p)...)

(define (move-x n p)
  (make-pos (+ n (pos-x p)) (pos-y p)))

(check-expect (move-x 1 (make-pos 1 2)) (make-pos 2 2))

;; move-y : num pos -> pos
;; adds a number num to the y-coordinate of pos
;; (define (move-y n p)...)

(define (move-y n p)
  (make-pos (pos-x p) (+ n (pos-y p))))

(check-expect (move-y 1 (make-pos 1 2)) (make-pos 1 3))

;; move-cloud-x : num cloud -> cloud
;; moves a cloud c i pixels along the x-axis
;; (define (move-cloud-x i c)...)

(define (move-cloud-x i c)
  (make-cloud (cloud-size c) (move-x i (cloud-pos c))))

;; tick : world -> world
;; manages the time-step changes to the state of the world
;; (define (tick w)...)

(define (tick w)
  ;; the conds decide if the cloud has left the scene, and if so, move it back to the right-side.
  ;; since the clouds are different sizes and move at different speeds, using constant values for the placement gives a pseudo-random effect.
  (make-world (move-cloud-x (cond
                              [(< (pos-x (cloud-pos (world-c1 w))) -200) (+ bg-width 300)]
                              [else -1.5 ])
                            (world-c1 w))
              (move-cloud-x (cond
                              [(< (pos-x (cloud-pos (world-c2 w))) -200) (+ bg-width 300)]
                              [else -1.2 ])
                            (world-c2 w))
              (move-cloud-x (cond
                              [(< (pos-x (cloud-pos (world-c3 w))) -200) (+ bg-width 300)]
                              [else -1 ])
                            (world-c3 w))
              (world-sun w)))

;; (big-bang init-world (on-tick tick) (to-draw render))

;; Part 3

;; I made the clouds loop and I added a sunset key. I wanted to change the color of the sun and the sky on sunset, but this would require changing the world struct to include these color values and then changing most of the functions in here (background, place-sun, render, etc.). I'm not that ambitious.

;; move-cloud-y : num cloud -> cloud
;; moves a cloud c i pixels along the y-axis
;; (define (move-cloud-y i c)...)

(define (move-cloud-y i c)
  (make-cloud (cloud-size c) (move-y i (cloud-pos c))))

;; read-key : world key -> world
;; takes key inputs and changes the world accordingly
;; (define (read-key w k)...)

(define (read-key w k)
  (cond
    [(key=? k "up")
     (make-world (move-cloud-y 1.5 (world-c1 w))
                 (move-cloud-y 1.2 (world-c2 w))
                 (move-cloud-y 1 (world-c3 w))
                 (world-sun w))]
    [(key=? k "down")
     (make-world (move-cloud-y -1.5 (world-c1 w))
                 (move-cloud-y -1.2 (world-c2 w))
                 (move-cloud-y -1 (world-c3 w))
                 (world-sun w))]
    [(key=? k "s")
     ;; makes a new world with the same values as before, except the sun is moved. If the sun is in the initial position, it gets moved 200 pixels down. Otherwise it gets put back to the initial state.
     (cond
       [(= (pos-y (sun-pos (world-sun w))) (pos-y (sun-pos init-sun)))
        (make-world (world-c1 w)
                    (world-c2 w)
                    (world-c3 w)
                    (make-sun init-sun-size (make-pos (pos-x (sun-pos (world-sun w))) 200)))]
       [else (make-world (world-c1 w)
                         (world-c2 w)
                         (world-c3 w)
                         (make-sun init-sun-size (make-pos (pos-x (sun-pos (world-sun w))) (pos-y (sun-pos init-sun)))))])]
    [else (make-world (world-c1 w)
                      (world-c2 w)
                      (world-c3 w)
                      (world-sun w))]))

(big-bang init-world (on-tick tick) (to-draw render) (on-key read-key))