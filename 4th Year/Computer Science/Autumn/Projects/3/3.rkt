;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname |3|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;; Kris Harper
;; Project 2
;; CMSC 15100
;; 12.8.10

;;;;;;;;;; requirements
;;
(require htdp/matrix)
(require 2htdp/image)
(require 2htdp/universe)
(require 2htdp/batch-io)


;;;;;;;;;; Priority queues based on leftist trees
;;

(define-struct priority-queue
  (priority  ;; : X -> num     -- function for computing priority.
   nitems    ;; : nat          -- the number of items in the queue
   heap))    ;; : (pq-node X)  -- a leftist-tree representation of a heap

;; a heap is either empty (represented as #f) or a pq-node
(define-struct pq-node
  (rank      ;; : nat          -- 
   item      ;; : X            -- the heap item at this node
   l         ;; : (pq-node X)  -- left child
   r))       ;; : (pq-node X)  -- right child

;; pq-heap-singleton : X -> (pq-heap X)
(define (pq-heap-singleton item) (make-pq-node 1 item #f #f))

;; pq-heap-rank : (pq-heap X) -> nat
(define (pq-heap-rank h)
  (cond
    [(pq-node? h) (pq-node-rank h)]
    [else 0]))

;; pq-heap-merge : (X -> num) (pq-heap X) (pq-heap X) -> (pq-heap X)
;; merge two priority-queue heaps
(define (pq-heap-merge priority-fn h1 h2)
  (local
    ((define (priority nd) (priority-fn (pq-node-item nd)))
     (define (mk-node item a b)
       (cond
         [(<= (pq-heap-rank a) (pq-heap-rank b)) (make-pq-node (+ (pq-heap-rank b) 1) item a b)]
         [else (make-pq-node (+ (pq-heap-rank a) 1) item b a)]))
     (define (merge h1 h2)
       (cond
         [(equal? h1 #f) h2]
         [(equal? h2 #f) h1]
         [(<= (priority h1) (priority h2))
          (mk-node (pq-node-item h1) (pq-node-l h1) (merge (pq-node-r h1) h2))]
         [else (mk-node (pq-node-item h2)(pq-node-l h2) (merge h1 (pq-node-r h2)))])))
    (merge h1 h2)))
             
;; pq-insert : X (priority-queue X) -> void
;; insert an item into a priority queue
(define (pq-insert item pq)
  (begin
    (set-priority-queue-nitems! pq (+ (priority-queue-nitems pq) 1))
    (set-priority-queue-heap!
     pq
     (pq-heap-merge (priority-queue-priority pq) (pq-heap-singleton item) (priority-queue-heap pq)))))

;; pq-new : (X -> num) -> (priority-queue X)
;; Create a new empty priority queue that uses priority-fn to order
;; the elements of the queue
(define (pq-new priority-fn) (make-priority-queue priority-fn 0 #f))

;; pq-next : (priority-queue X) -> (oneof X #f)
;; remove and return the item with the lowest priority from the queue
(define (pq-next pq)
  (cond
    [(pq-node? (priority-queue-heap pq))
     (local
       ((define root (priority-queue-heap pq)))
       (begin
         (set-priority-queue-nitems! pq (- (priority-queue-nitems pq) 1))
         (set-priority-queue-heap!
          pq (pq-heap-merge (priority-queue-priority pq) (pq-node-l root) (pq-node-r root)))
         (pq-node-item root)))]
    [else #f]))

;; pq-count : (priority-queue X) -> nat
;; return a count of the number of items in the priority queue
(define pq-count priority-queue-nitems)

;; pq-empty? : (priority-queue X) -> boolean
;; return #t if the priority queue is empty
(define (pq-empty? pq) (= (pq-count pq) 0))

(check-expect
 (local ((define pq (pq-new (lambda (x) x))))
   (begin
     (for-each (lambda (x) (pq-insert x pq)) '(17 12 42 1))
     (list (pq-next pq) (pq-next pq) (pq-next pq) (pq-next pq) (pq-next pq))))
 (list 1 12 17 42 #f))



;;;;;;;;;; Mazes
;;

;; A location on a map is
(define-struct location
  (row      ;; : nat    -- the location's row
   col))    ;; : nat    -- the location's column

;; location-add : location location -> location
;; location addition
(define (location-add loc1 loc2)
  (make-location
   (+ (location-row loc1) (location-row loc2))
   (+ (location-col loc1) (location-col loc2))))

;; a tile is either a 'wall or 'open
  
;; A maze is
(define-struct maze
  (name     ;; : string         -- the name of the maze
   tiles    ;; : (matrix tile)  -- a matrix of tiles
   start    ;; : location       -- the start location
   goal))   ;; : location       -- the goal location

;; matrix->maze : string (matrix tile) -> maze
(define (matrix->maze name tiles)
  (local
    ((define (posn->loc p)
       (cond
         [(and (cons? p) (empty? (rest p))) (make-location (posn-x (first p)) (posn-y (first p)))]
         [else (error 'load-maze "maze must have exactly one start and goal tile")]))
     (define start-posn (posn->loc (matrix-where? tiles (lambda (tile) (symbol=? tile 'start)))))
     (define goal-posn (posn->loc (matrix-where? tiles (lambda (tile) (symbol=? tile 'goal))))))
    (make-maze name tiles start-posn goal-posn)))

;; load-maze : string -> maze
;; load a maze from a file
(define (load-maze filename)
  (local
    ((define lines (read-lines filename))
     (define ncols (string-length (first lines)))
     (define (char->tile chr)
       (cond
         [(char=? #\O chr) 'open]
         [(char=? #\W chr) 'wall]
         [(char=? #\S chr) 'start]
         [(char=? #\G chr) 'goal]
         [else (error 'load-maze "invalid input character")]))
     (define (line->lot ln) (map char->tile (string->list ln)))
     (define tiles (rectangle->matrix (map line->lot lines))))
    (matrix->maze filename tiles)))

;; tile-ref : maze -> nat nat -> tile
(define (tile-ref maze)
  (local
    ((define tiles (maze-tiles maze)))
    (lambda (r c)
      (cond
        [(and (< -1 r (matrix-rows tiles)) (< -1 c (matrix-cols tiles))) (matrix-ref tiles r c)]
        [else 'wall]))))

;; tile-location-ref : maze -> location -> tile
(define (tile-location-ref maze)
  (local
    ((define tile (tile-ref maze)))
    (lambda (loc) (tile (location-row loc) (location-col loc)))))

;; compass-point offsets
(define compass-offsets
  (list
   (make-location -1  0)    ;; north
   (make-location  0  1)    ;; east
   (make-location  1  0)    ;; south
   (make-location  0 -1)))  ;; west

;; neighbors : maze -> location -> (list location)
;; return a list of the non-wall neighbors of a tile
(define (neighbors maze)
  (local
    ((define tile (tile-location-ref maze))
     (define (mk-neighbors loc)
       (local
         ((define (add offset l)
            (cond
              [(symbol=? (tile (location-add loc offset)) 'wall) l]
              [else (cons (location-add loc offset) l)])))
         (foldl add '() compass-offsets))))
    mk-neighbors))

;; the size of a tile
(define tile-size 24)
(define tile-half-size (/ tile-size 2))

;; tile icons
(define start-icon (square 18 "solid" "green"))
(define goal-icon (triangle 18 "solid" "red"))
(define open-icon (circle 7 "outline" "blue"))
(define closed-icon (square 10 "solid" "blue"))

;; size of title font
(define title-font-size 16)

;; render-maze : string maze -> image
;; render a maze and its title as an image
(define (render-maze title maze)
  (local
    ((define wall (square tile-size "solid" "DarkGray"))
     (define open (square tile-size "outline" "black"))
     (define wid (matrix-cols (maze-tiles maze)))
     (define ht  (matrix-rows (maze-tiles maze)))
     (define tile (tile-ref maze))
     (define (render r c)
       (local
         ((define t (tile r c)))
         (cond
           [(symbol=? 'wall t) wall]
           [(symbol=? 'open t) open]
           [(symbol=? 'start t) (overlay start-icon open)]
           [(symbol=? 'goal t) (overlay goal-icon open)])))
     (define (row-lp r img)
       (local
         ((define (col-lp c row-img)
            (cond
              [(< c wid) (col-lp (+ c 1) (beside row-img (render r c)))]
              [else row-img])))
         (cond
           [(< r ht) (row-lp (+ r 1) (above img (col-lp 1 (render r 0))))]
           [else img]))))
    (row-lp 0 (text title title-font-size "black"))))

;; overlay-tile : image location image -> image
;; overlay image over on image under centered on the tile with the given location.
(define (overlay-tile over loc under)
  (local
    ((define y (+ (* tile-size (location-row loc)) title-font-size (* 0.5 (- tile-size (image-height over)))))
     (define x (+ (* tile-size (location-col loc)) (* 0.5 (- tile-size (image-width over))))))
    (underlay/xy under x y over)))

(define path-pen (make-pen "purple" 3 "solid" "round" "round"))

(define (location->x loc) (+ (* tile-size (location-col loc)) tile-half-size))
(define (location->y loc) (+ (* tile-size (location-row loc)) tile-half-size title-font-size))

;; render-edge : image location location -> image
;; render a path edge between the two locations over the given image
(define (render-edge img loc1 loc2)
  (scene+line img (location->x loc1) (location->y loc1) (location->x loc2) (location->y loc2) path-pen))

;; some tests
(define test-maze
  (matrix->maze
   "test-maze"
   (rectangle->matrix
    '((wall wall  wall wall wall)
      (wall start open open wall)
      (wall open  open open wall)
      (wall open  open goal wall)
      (wall wall  wall wall wall)))))

;; (render-maze "test-maze" test-maze)

(check-expect ((tile-ref test-maze) 1 1) 'start)
(check-expect ((tile-ref test-maze) 3 4) 'wall)
(check-expect ((tile-ref test-maze) 2 2) 'open)
(check-expect ((neighbors test-maze) (make-location 2 2))
              (list
               (make-location 2 1)   ; west
               (make-location 3 2)   ; south
               (make-location 2 3)   ; east
               (make-location 1 2))) ; north
(check-expect ((neighbors test-maze) (make-location 1 1))
              (list
               (make-location 2 1)   ; south
               (make-location 1 2))) ; east


;;;;;;;;;;;;;;;;;;;; your code starts here ;;;;;;;;;;;;;;;;;;;;
;;

;; The Graph

;; a tile-node-status is either
;;  - 'open
;;  - 'closed

;; a tile-node is a (make-tile-node loc stat cost prev) where
;;  - loc is a location
;;  - stat is a tile-node-status
;;  - cost is a num
;;  - prev is either
;;    - #f
;;    - tile-node

(define-struct tile-node (loc stat cost prev))

;; an open set is a priority queue of tile-nodes


;; The World

;; a world is a (make-world maze open table count mode) where
;;  - maze is a maze
;;  - open is a priority queue
;;  - table is a hash table
;;  - count is a number
;;  - mode is one of
;;    - 'stepping
;;    - 'running
;;    - 'success
;;    - 'stuck

(define-struct world (maze open table count mode))

;; world-done? : world -> boolean
;; return true if the search is finished; i.e., the mode is 'success or 'stuck.
;; (define (world-done? wrld)...)

(define (world-done? wrld)
  (cond
    [(not (world? wrld)) (error 'world-done? "world expected")]
    [else (or (equal? (world-mode wrld) 'success)
              (equal? (world-mode wrld) 'stuck))]))

(check-expect (world-done? (make-world test-maze
                                       (pq-new (lambda (x) x))
                                       (make-hash empty)
                                       0
                                       'stepping)) #f)
(check-expect (world-done? (make-world test-maze
                                       (pq-new (lambda (x) x))
                                       (make-hash empty)
                                       0
                                       'success)) #t)
(check-error (world-done? 0) "world-done?: world expected")

;; distance : loc1 loc2 -> num
;; the taxicab metric
;; (define (distance loc1 loc2)...)

(define (distance loc1 loc2)
  (cond
    [(or (not (location? loc1)) (not (location? loc2)))
     (error 'distance "location expected")]
    [else (+ (abs (- (location-row loc1) (location-row loc2)))
             (abs (- (location-col loc1) (location-col loc2))))]))

(check-expect (distance (make-location 0 0) (make-location 1 1)) 2)
(check-expect (distance (make-location 1 2) (make-location 3 4)) 4)
(check-error (distance 0 0) "distance: location expected")

;; new-world : maze -> world
;; create and initialize new world for the given maze.
;; (define (new-world maze)...)

(define (new-world maze)
  (cond
    [(not (maze? maze)) (error 'new-world "maze expected")]
    [else (local ((define open-set
                    (pq-new
                     (lambda (x) (+ (tile-node-cost x)
                                    (distance (tile-node-loc x)
                                              (maze-goal maze))))))
                  (define start (make-tile-node (maze-start maze)
                                                'open
                                                0
                                                #f))
                  (define hash (make-hash (list
                                           (list (maze-start maze)
                                                 start)))))
            (make-world maze
                        (begin
                          (pq-insert start open-set)
                          open-set)
                        hash
                        0
                        'stepping))]))

(check-expect (world? (new-world test-maze)) #t)
(check-error (new-world 0) "new-world: maze expected")


;; Implementing A*

;; process-neghbor : world tile-node -> (list of locations) -> void
;; check neighboring locations to the tile-node nd. This includes checking
;;   to see if there is already a tile-node for the location and, if not, creating
;;   one and adding it to the open set.
;; (define (process-neighbor wrld nd)...)

(define (process-neighbor wrld nd)
  (cond
    [(not (world? wrld)) (error 'process-neighbor "world expected")]
    [(not (tile-node? nd)) (error 'process-neighbor "tile-node expected")]
    [else (lambda (locs)
            (for-each
             (lambda (loc)
               (cond
                 [(not (hash-has-key? (world-table wrld) loc))
                  (local ((define curr-node (make-tile-node
                                             loc
                                             'open
                                             (+ 1 (tile-node-cost nd))
                                             nd)))
                    (begin
                      (pq-insert curr-node (world-open wrld))
                      (hash-set! (world-table wrld) loc curr-node)))]
                 [(and (equal?
                        (tile-node-stat (hash-ref (world-table wrld) loc))
                        'open)
                       (< (+ (tile-node-cost
                              (hash-ref (world-table wrld) loc)) 1)
                          (tile-node-cost nd)))
                  (begin
                    (set-tile-node-cost! (hash-ref (world-table wrld) loc)
                                         (+ (tile-node-cost nd) 1))
                    (set-tile-node-prev! (hash-ref (world-table wrld) loc)
                                         nd)
                    (pq-insert (hash-ref (world-table wrld) loc)
                               (world-open wrld)))]
                 [else void]))
             locs))]))

(check-error (process-neighbor 0 (make-tile-node 0 0 0 0))
             "process-neighbor: world expected")
(check-error (process-neighbor (new-world test-maze) 0)
             "process-neighbor: tile-node expected")
            

;; A*-step : world -> world
;; Take one step of the A* algorithm. Note that we assume that the world
;; is either in 'stepping or 'running mode.
;; (define (A*-step wrld)...)

(define (A*-step wrld)
  (cond
    [(not (world? wrld)) (error 'A*-step "world expected")]
    [else (cond
            [(empty? (world-open wrld)) (begin
                                          (set-world-mode! wrld 'stuck)
                                          wrld)]
            [else
             (local ((define node (pq-next (world-open wrld))))
               (cond
                 [(equal? node #f)
                  (begin
                    (set-world-mode! wrld 'stuck)
                    wrld)]
                 [else
                  (begin
                    (cond
                      [(equal? (tile-node-stat node) 'closed) (A*-step wrld)]
                      [else (set-tile-node-stat! node 'closed)])
                    (cond
                      [(equal? (tile-node-loc node)
                               (maze-goal (world-maze wrld)))
                       (begin
                         (set-world-count! wrld (+ 1 (world-count wrld)))
                         (set-world-mode! wrld 'success)
                         wrld)]
                      [else (begin
                              ((process-neighbor wrld node)
                               ((neighbors (world-maze wrld))
                                (tile-node-loc node)))
                              (set-world-count! wrld (+ 1(world-count wrld)))
                              wrld)]))]))])]))

(check-error (A*-step 0) "A*-step: world expected")


;; User interface

;; render-world : world -> image
;; render the world as an image.
;; (define (render-world wrld)...)

(define (render-world wrld)
  (cond
    [(not (world? wrld)) (error 'render-world "world expected")]
    [else (local ((define visited (hash-map (world-table wrld)
                                            (lambda (x y) y)))
                  (define (render l)
                    (cond
                      [(empty? l) (render-maze
                                   (string-append
                                    (maze-name (world-maze wrld))
                                    ": step "
                                    (number->string (world-count wrld))
                                    ", "
                                    (number->string (pq-count (world-open wrld)))
                                    " open node(s)")
                                   (world-maze wrld))]
                      [else (cond
                              [(equal? (tile-node-stat (first l)) 'closed)
                               (overlay-tile closed-icon
                                             (tile-node-loc (first l))
                                             (render (rest l)))]
                              [(equal? (tile-node-stat (first l)) 'open)
                               (overlay-tile open-icon
                                             (tile-node-loc (first l))
                                             (render (rest l)))])])))
            (cond
              [(equal? (world-mode wrld) 'success)
               (render-path (render visited)
                            (hash-ref (world-table wrld)
                                      (maze-goal (world-maze wrld))))]
              [else (render visited)]))]))

(check-error (render-world 0) "render-world: world expected")

;; render-path : image tile-node -> image
;; render the path from the node nd (presumably the goal) to the start node
;; (define (render-path img nd)...)

(define (render-path img nd)
  (cond
    [(not (image? img)) (error 'render-path "image expected")]
    [(not (tile-node? nd)) (error 'render-path "tile-node expected")]
    [else (cond
            [(equal? (tile-node-prev nd) #f) img]
            [else (render-edge (render-path img (tile-node-prev nd))
                               (tile-node-loc nd)
                               (tile-node-loc (tile-node-prev nd)))])]))

(check-error (render-path 0 (make-tile-node 0 0 0 0))
             "render-path: image expected")
(check-error (render-path (rectangle 0 0 "solid" "white") 0)
             "render-path: tile-node expected")
                                  

;; step-on-tick : world -> world
;; determines the new state of the world given a current state
;; (define (step-on-tick w)...)

(define (step-on-tick w)
  (cond
    [(not (world? w)) (error 'step-on-tick "world expected")]
    [else (cond
            [(equal? (world-mode w) 'running) (A*-step w)]
            [else w])]))

(check-error (step-on-tick 0) "step-on-tick: world expected")

;; step-on-key : world key -> world
;; decides the output of user key inputs
;; (define (step-on-key w k)...)

(define (step-on-key w k)
  (cond
    [(not (world? w)) (error 'step-on-key "world expected")]
    [(not (key-event? k)) (error 'step-on-key "key expected")]
    [else (cond
            [(key=? "r" k)
             (cond
               [(equal? (world-mode w) 'stepping)
                (begin
                  (set-world-mode! w 'running)
                  w)]
               [else w])]
            [(key=? "s" k)
             (cond
               [(equal? (world-mode w) 'running)
                (begin
                  (set-world-mode! w 'stepping)
                  w)]
               [else w])]
            [(key=? " " k)
             (cond
               [(equal? (world-mode w) 'stepping) (A*-step w)]
               [else w])]
            [else w])]))

(check-error (step-on-key 0 "a") "step-on-key: world expected")
(check-error (step-on-key (new-world test-maze) 0) "step-on-key: key expected")

;; run : string -> void
;; Run the A* animation on the maze defined by the named file.
(define (run filename)
  (begin
    (big-bang (new-world (load-maze filename))
              (on-tick step-on-tick 1/5)
              (on-key step-on-key)
              (to-draw render-world)
              (stop-when world-done?))
    (void)))