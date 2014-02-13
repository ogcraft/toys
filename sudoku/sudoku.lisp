(defpackage :sudoku
  (:use :common-lisp))

(in-package :sudoku)

(ql:quickload "alexandria")

; start code here
; 
;digits   = '123456789'
;rows     = 'ABCDEFGHI'
;cols     = digits

(defconstant +digits+ '("1" "2" "3" "4" "5" "6" "7" "8" "9"))
(defconstant +rows+ '("A" "B" "C" "D" "E" "F" "G" "H" "I"))
(defconstant +cols+ +digits+)

(defun strconcat (x y) (concatenate 'string x y)
(defun cross ( x y ) (alexandria:map-product 'strconcat x b))



