;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname |1|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;; CS 151, University of Chicago
;; Kris Harper
;; Sept 2010
;; Lab 1

(require 2htdp/image)

;; John Hancock Center ("jhc"):
;; 344 m tall
;; with antennas, 457 m
;; thus antennas are 113 m

;; JHC dimensions from http://en.wikipedia.org/wiki/John_Hancock_Center.

;; named constants
;; all numbers 1 meter per pixel
(define mid-h 339) ;; middle part height
(define mid-w 44)  ;; middle part width
(define tri-w 12)  ;; triangle width
(define top-h 5)   ;; topper height
(define top-w 37)  ;; topper width
(define ant-h 113) ;; antenna height

;; jhc components

;; jhc-middle : img
(define jhc-middle
  (rectangle mid-w mid-h "solid" "black"))

;; jhc-rside : img
(define jhc-rside
  (right-triangle tri-w mid-h "solid" "black"))

;; jhc-lside : img
(define jhc-lside
  (flip-horizontal jhc-rside))

;; jhc-topper : img
(define jhc-topper
  (rectangle top-w top-h "solid" "black"))

;; antenna : img
(define antenna
  (rectangle 1 ant-h "outline" "gray"))

;; jhc : img
;; add the pieces
(define jhc
  (above/align "center"
               (beside/align "bottom"
                             antenna
                             (rectangle (- top-w 2) 1 "solid" "white")
                             antenna)
               (above/align "center"
                            jhc-topper
                            (beside/align "bottom"
                                          jhc-lside
                                          jhc-middle
                                          jhc-rside))))

(check-expect (image-height jhc) 457)

;; scalable dimension functions

;; s-jhc-middle : num -> img
(define (s-jhc-middle s-factor) (rectangle (/ mid-w s-factor) (/ mid-h s-factor) "solid" "black"))

;; s-jhc-rside : num -> img
(define (s-jhc-rside s-factor) (right-triangle (/ tri-w s-factor) (/ mid-h s-factor) "solid" "black"))

;; s-jhc-lside : num -> img
(define (s-jhc-lside s-factor) (flip-horizontal (s-jhc-rside s-factor)))

;; s-jhc-topper : num -> img
(define (s-jhc-topper s-factor) (rectangle (/ top-w s-factor) (/ top-h s-factor) "solid" "black"))

;; s-jhc-antenna : num -> img
(define (s-jhc-antenna s-factor) (rectangle (/ 1 s-factor) (/ ant-h s-factor) "outline" "gray"))

;; s-jhc-top-mid-w : num -> num
(define (s-jhc-top-mid-w s-factor) (- (/ top-w s-factor) (* (/ 1 s-factor) 2)))

;; add the scaled pieces together
;; s-jhc : num -> img
(define (s-jhc s-factor)
  (above/align "center"
               (beside/align "bottom"
                             (s-jhc-antenna s-factor)
                             (rectangle (s-jhc-top-mid-w s-factor) 1 "solid" "white")
                             (s-jhc-antenna s-factor))
               (above/align "center"
                            (s-jhc-topper s-factor)
                            (beside/align "bottom"
                                          (s-jhc-lside s-factor)
                                          (s-jhc-middle s-factor)
                                          (s-jhc-rside s-factor)))))

(check-expect (image-height (s-jhc .5)) (* 457 2))


;; Smurfit-Stone Building ("ssb"):
;; 177 m tall

;; SSB dimensions from http://en.wikipedia.org/wiki/Smurfit-Stone_Building.

;; named constants
;; all numbers 1 meter per pixel

;; middle part height
(define ssb-mid-h 101)

;; middle part width
(define ssb-mid-w 4)

;; left side width
(define ssb-left-w 18)

;; triangle height
(define ssb-tri-h 38)

;; s-ssb-middle : num -> img
(define (s-ssb-middle s-factor) (rectangle (/ ssb-mid-w s-factor) (/ ssb-mid-h s-factor) "solid" "lightgray"))

;; s-ssb-left : num -> img
(define (s-ssb-left s-factor) (rectangle (/ ssb-left-w s-factor) (/ ssb-mid-h s-factor) "solid" "gray"))

;; s-ssb-bottom-tri-left : num -> img
(define (s-ssb-bottom-tri-left s-factor) (right-triangle (/ ssb-left-w s-factor) (/ ssb-tri-h s-factor) "solid" "gray"))

;; s-ssb-top-tri-right : num -> img
(define (s-ssb-top-tri-right s-factor) (right-triangle (/ ssb-left-w s-factor) (/ ssb-tri-h s-factor) "solid" "darkgray"))

;; s-ssb-top-space : num -> img
(define (s-ssb-top-space s-factor) (rectangle (/ ssb-mid-w s-factor) 1 "solid" "white"))

;; part3-building : num -> img
;; puts all the pieces together
(define (part3-building s-factor)
  (above/align "center"
               (beside/align "bottom"
                             (flip-horizontal (s-ssb-top-tri-right s-factor))
                             (s-ssb-top-space s-factor)
                             (s-ssb-top-tri-right s-factor))
               (beside/align "bottom"
                             (overlay
                              (s-ssb-bottom-tri-left s-factor)
                              (flip-horizontal (flip-vertical (s-ssb-top-tri-right s-factor))))
                             (s-ssb-top-space s-factor)
                             (overlay
                              (flip-vertical (s-ssb-top-tri-right s-factor))
                              (flip-horizontal (s-ssb-bottom-tri-left s-factor))))
               (beside/align "bottom"
                             (s-ssb-left s-factor)
                             (s-ssb-middle s-factor)
                             (s-ssb-left s-factor))))

(check-expect (image-height (part3-building .5)) (* 177 2))