;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname |1|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; Kris Harper
;; CMSC 15100
;; 10.05.10
;; Homework 1
(require 2htdp/image)

;; Exercise 1

;; defined constant for the value pi
(define PI 3.14)

;; area : num num -> num
;; takes the radius and height of a cylinder and gives the surface area
;; (define (area r h)...)
(define (area r h)
  (+ (* 2 (* (* r r) PI)) (* (* (* 2 PI) r) h)))

(check-expect (area 5 10) 471)


;; Exercise 2

;; power : num int -> num
;; takes a number x and a positive integer n and returns x^n
;; (define (power x n)...)
(define (power x n)
  (cond
    [(or (<= n 0) (not (integer? n))) (error 'power "Please enter a positive integer")]
    [(= n 1) x]
    [else (* x (power x (- n 1)))]))

(check-expect (power 2 5) 32)
(check-expect (power 0 8) 0)
(check-expect (power (- 3) 3) (- 27))
(check-expect (power 2.1 4) 19.4481)
  

;; Exercise 3

;; face : string -> img
;; takes a color and makes a face with that eye color
;; (define (face color)...)
(define (face color)
  (overlay/xy
   (circle 100 "outline" "black")
   55 60
   (overlay/xy
    (circle 10 "solid" color)
    -75 0
    (overlay/xy
     (circle 10 "solid" color)
     25 60
     (rectangle 40 10 "solid" "red")))))

(check-expect (image-width (face "blue")) 200)

;; Exercise 4

;; bullseye : string string -> img
;; takes two colors and makes a bullseye with those colors
;; (define (bullseye color1 color2)...)
(define (bullseye color1 color2)
  (overlay
   (circle 20 "solid" color1)
   (circle 40 "solid" color2)
   (circle 60 "solid" color1)
   (circle 80 "solid" color2)
   (circle 100 "solid" color1)))

(check-expect (image-width (bullseye "red" "white")) 200)

;; Exercise 5

;; wide-image : img img img -> img
;; takes three images and finds the widest one
;; if two arguments have the same width it returns the one which comes first in the argument list
;; (define (wide-image image1 image2 image3)...)
(define (wide-image image1 image2 image3)
  (cond
    [(or (<= (image-width image2) (image-width image3) (image-width image1)) (<= (image-width image3) (image-width image2) (image-width image1))) image1]
    [(or (<= (image-width image1) (image-width image3) (image-width image2)) (<= (image-width image3) (image-width image1) (image-width image2))) image2]
    [(or (<= (image-width image1) (image-width image2) (image-width image3)) (<= (image-width image2) (image-width image1) (image-width image3))) image3]))

(check-expect (image-width (wide-image (rectangle 10 10 "solid" "red") (rectangle 20 10 "solid" "blue") (rectangle 30 10 "solid" "green"))) 30)
(check-expect (image-width (wide-image (rectangle 10 10 "solid" "red") (rectangle 30 10 "solid" "blue") (rectangle 20 10 "solid" "green"))) 30)
(check-expect (image-width (wide-image (rectangle 20 10 "solid" "red") (rectangle 10 10 "solid" "blue") (rectangle 30 10 "solid" "green"))) 30)
(check-expect (image-width (wide-image (rectangle 20 10 "solid" "red") (rectangle 30 10 "solid" "blue") (rectangle 10 10 "solid" "green"))) 30)
(check-expect (image-width (wide-image (rectangle 30 10 "solid" "red") (rectangle 10 10 "solid" "blue") (rectangle 20 10 "solid" "green"))) 30)
(check-expect (image-width (wide-image (rectangle 30 10 "solid" "red") (rectangle 20 10 "solid" "blue") (rectangle 10 10 "solid" "green"))) 30)

;; middle-image : img img img -> img
;; takes three images and finds the middle one
;; if two arguments have the same width it returns the one which comes first in the argument list
;; (define (middle-image image1 image2 image3)...)
(define (middle-image image1 image2 image3)
  (cond
    [(or (<= (image-width image2) (image-width image1) (image-width image3)) (<= (image-width image3) (image-width image1) (image-width image2))) image1]
    [(or (<= (image-width image1) (image-width image2) (image-width image3)) (<= (image-width image3) (image-width image2) (image-width image1))) image2]
    [(or (<= (image-width image1) (image-width image3) (image-width image2)) (<= (image-width image2) (image-width image3) (image-width image1))) image3]))

(check-expect (image-width (middle-image (rectangle 10 10 "solid" "red") (rectangle 20 10 "solid" "blue") (rectangle 30 10 "solid" "green"))) 20)
(check-expect (image-width (middle-image (rectangle 10 10 "solid" "red") (rectangle 30 10 "solid" "blue") (rectangle 20 10 "solid" "green"))) 20)
(check-expect (image-width (middle-image (rectangle 20 10 "solid" "red") (rectangle 10 10 "solid" "blue") (rectangle 30 10 "solid" "green"))) 20)
(check-expect (image-width (middle-image (rectangle 20 10 "solid" "red") (rectangle 30 10 "solid" "blue") (rectangle 10 10 "solid" "green"))) 20)
(check-expect (image-width (middle-image (rectangle 30 10 "solid" "red") (rectangle 10 10 "solid" "blue") (rectangle 20 10 "solid" "green"))) 20)
(check-expect (image-width (middle-image (rectangle 30 10 "solid" "red") (rectangle 20 10 "solid" "blue") (rectangle 10 10 "solid" "green"))) 20)

;; narrow-image : img img img -> img
;; takes three images and finds the narrowest one
;; if two arguments have the same width it returns the one which comes first in the argument list
;; (define (narrow-image image1 image2 image3)...)
(define (narrow-image image1 image2 image3)
  (cond
    [(or (<= (image-width image1) (image-width image2) (image-width image3)) (<= (image-width image1) (image-width image3) (image-width image2))) image1]
    [(or (<= (image-width image2) (image-width image1) (image-width image3)) (<= (image-width image2) (image-width image3) (image-width image1))) image2]
    [(or (<= (image-width image3) (image-width image1) (image-width image2)) (<= (image-width image3) (image-width image2) (image-width image1))) image3]))

(check-expect (image-width (narrow-image (rectangle 10 10 "solid" "red") (rectangle 20 10 "solid" "blue") (rectangle 30 10 "solid" "green"))) 10)
(check-expect (image-width (narrow-image (rectangle 10 10 "solid" "red") (rectangle 30 10 "solid" "blue") (rectangle 20 10 "solid" "green"))) 10)
(check-expect (image-width (narrow-image (rectangle 20 10 "solid" "red") (rectangle 10 10 "solid" "blue") (rectangle 30 10 "solid" "green"))) 10)
(check-expect (image-width (narrow-image (rectangle 20 10 "solid" "red") (rectangle 30 10 "solid" "blue") (rectangle 10 10 "solid" "green"))) 10)
(check-expect (image-width (narrow-image (rectangle 30 10 "solid" "red") (rectangle 10 10 "solid" "blue") (rectangle 20 10 "solid" "green"))) 10)
(check-expect (image-width (narrow-image (rectangle 30 10 "solid" "red") (rectangle 20 10 "solid" "blue") (rectangle 10 10 "solid" "green"))) 10)

;; stack : img img img -> img
;; takes three images and stacks them in descending order based on their width.
;; I've changed the order of inputs in each of the three functions to take care
;; of the possibility that two inputs have the same width.
;; (define (stack image1 image2 image2)...)
(define (stack image1 image2 image3)
  (above/align "center"
               (narrow-image image1 image2 image3)
               (middle-image image2 image3 image1)
               (wide-image image3 image1 image2)))

(check-expect (image-width (stack (rectangle 10 10 "solid" "red") (rectangle 10 10 "solid" "blue") (rectangle 10 10 "solid" "green"))) 10)
(check-expect (image-height (stack (rectangle 10 10 "solid" "red") (rectangle 10 10 "solid" "blue") (rectangle 10 10 "solid" "green"))) 30)
(check-expect (image-width (stack (rectangle 10 10 "solid" "red") (rectangle 20 10 "solid" "blue") (rectangle 30 10 "solid" "green"))) 30)