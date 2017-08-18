(set-option :fixedpoint.engine datalog)
; This sort is used to define all relations.
(define-sort s () (_ BitVec 32))

; Dominance relation of CFG 
; (dom A B) means A is a dominator of B
(declare-rel dom (s s))

; Relation specifying that a certain load reads from a certain store
; (reads-from)
(declare-rel rf (s s))

; Relation specifying that a statement cannot read from another statement. This
; is derived from statements which are currently in rf and po
(declare-rel not-rf (s s))

; Dominance is transitive
(declare-var A s)
(declare-var B s)
(declare-var C s)
(declare-var X s)
(declare-var Y s)
(rule (=> (and (dom A B) (dom B C)) (dom A C)))

; The not-rf (not read from) relation. Given two pairs of dominators and a
; read-from, an item must not-read from another item if it would introduce a
; cycle
(rule (=> (and (dom A B) (rf A Y) (dom X Y)) (not-rf B X)))


;;; Begin Facts
(rule (dom #x00000001 #x00000002))
(rule (dom #x00000002 #x00000003))

(rule (dom #x00000004 #x00000005))
(rule (dom #x00000006 #x00000007))

(rule (dom #x00000005 #x0000000A))
(rule (dom #x00000005 #x00000006))
(rule (dom #x00000007 #x00000008))
(rule (dom #x00000007 #x00000009))

(rule (rf #x00000004 #x00000003))
(rule (rf #x00000006 #x00000001))

;;; End Facts

;;; Test Queries
; Entire cfg transitive closure
; (query (dom A B) :print-answer true)
; ; 7 dom 10? Should be UNSAT
; (query (dom #x00000007 #x0000000A))
; ; 5 dom 8? Should be SAT
; (query (dom #x00000005 #x00000008))
; (query (not-rf A B) :print-answer true)
(query (or (not-rf #x00000006 #x00000001) (not-rf #x00000004 #x00000005)))
