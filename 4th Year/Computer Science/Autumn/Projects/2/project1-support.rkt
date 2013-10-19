#lang racket/base
(require 2htdp/image
         racket/gui/base
         racket/class)
(provide build-image name->color)

;; build-image : nat nat (nat nat -> color) -> image-snip%
(define (build-image w h f)
  (unless (exact-nonnegative-integer? w)
    (error 'build-image "expected an exact, nonnegative integer as the first argument, got ~e" w))
  (unless (exact-nonnegative-integer? h)
    (error 'build-image "expected an exact, nonnegative integer as the second argument, got ~e" h))
  (unless (and (procedure? f)
               (procedure-arity-includes? f 2))
    (error 'build-image "expected a procedure that expects two arguments as the third argument, got ~e" f))
  (define bmp (make-object bitmap% w h))
  (define bdc (make-object bitmap-dc% bmp))
  (let ([global-c (make-object color%)])
    (for ([i (in-range 0 w)])
      (for ([j (in-range 0 h)])
        (let* ([f-res (f j i)]
               [new-c-obj (to-color% f-res global-c)])
          (unless new-c-obj
            (error 'build-image "function argument returned a non-color, ~e, when called with ~e and ~e"
                   f-res i j))
          (send bdc set-pixel i j new-c-obj)))))
  (send bdc set-bitmap #f)
  (make-object image-snip% bmp))

;; to-color% : image-color? (is-a?/c color%) -> (is-a?/c color%) or #f
;; this either mutates c-obj to match c or returns an immutable
;; color that it gets from the color-database that matches c
;; (avoiding this allocation is savings of about 30% on the example below)
(define (to-color% c c-obj)
  (cond
    [(color? c)
     (send c-obj set (color-red c) (color-green c) (color-blue c))
     c-obj]
    [(string? c)
     (or (send the-color-database find-color c)
         (send the-color-database find-color "black"))]
    [(symbol? c)
     (or (send the-color-database find-color (symbol->string c))
         (send the-color-database find-color "black"))]
    [else #f]))

;; name->color : ((or string symbol) -> color
;; convert named colors to (make-color r g b) values
(define (name->color c)
  (let
    ([cc 
       (cond
         [(string? c)
          (or (send the-color-database find-color c)
              (send the-color-database find-color "black"))]
         [(symbol? c)
          (or (send the-color-database find-color (symbol->string c))
              (send the-color-database find-color "black"))]
         [else (error 'name->color "argument must be symbol or string")])])
    (make-color (send cc red) (send cc green) (send cc blue))))

;; examples as tests....
#;
(build-image
 100 100
 (λ (x y) (if (zero? (modulo (+ x y) 2))
              "orange"
              'purple)))

#;
(time
 (build-image
  255
  255
  (λ (x y) (make-color 0 x y))))
