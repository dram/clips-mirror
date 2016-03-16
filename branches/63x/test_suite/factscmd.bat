(clear)
(facts)                            ; facts
(assert (a) (b) (c) (d) (e) (f))
(facts)
(facts 1 2 3 4 5)
(facts "a")
(facts 1 a)
(facts 1 2 a)
(facts -1 5 2)
(facts 4 -1 2)
(facts 3 4 -2)
(facts 4 2)
(facts 5)
(facts 3 5)
(facts 2 6 2)
(save-facts)                       ; save-facts and load-facts
(save-facts 10)
(save-facts "Temp//factscmd.tmp" 10)
(save-facts "Temp//factscmd.tmp")
(clear)
(facts)
(load-facts)
(load-facts 10)
(load-facts "Temp//factscmd.tmp" 10)
(load-facts "Temp//factscmd.tmp")
(facts)
(clear)                            ; dependencies and dependents
(defrule foo1 (logical (a) (b)) => (assert (e)))
(defrule foo2 (logical (b)) (c) => (assert (e)))
(defrule foo3 (logical (d) (b) (a)) (c) => (assert (e)))
(defrule foo4 (logical (d) (b) (a) (c)) => (assert (e)))
(defrule foo5 (logical (a) (c) (d)) => (assert (e)))
(defrule foo6 (declare (salience -1)) (logical (c) (d)) => (assert (f)))
(defrule foo7 (declare (salience -1)) (logical (a)) (c) => (assert (f)))
(defrule foo8 (declare (salience -1)) (logical (c) (a)) (d) => (assert (f)))
(defrule foo9 (declare (salience -2)) (logical (d) (c) (b)) => (assert (g)))
(assert (a) (b) (c) (d))
(run)
(facts)
(dependencies)
(dependencies a)
(dependencies 10)
(dependencies 5 6)
(dependencies 5)
(dependencies 3)
(dependents)
(dependents a)
(dependents 10)
(dependents 5 6)
(dependents 1)
(dependents 2)
(dependents 3)
(dependents 4)
(dependents 5)
(dependents 6)
(clear)                            ; get-fact-duplication and set-fact-duplication
(assert (a))
(assert (a))
(facts)
(get-fact-duplication)  
(get-fact-duplication TRUE)
(set-fact-duplication)
(set-fact-duplication TRUE FALSE)
(set-fact-duplication 10)
(get-fact-duplication)
(set-fact-duplication TRUE)
(get-fact-duplication)
(clear)
(assert (a))
(assert (a))
(facts)
(set-fact-duplication FALSE)
(get-fact-duplication)
(clear)
(assert (a))
(assert (a))
(facts)
(clear) ;; New 6.0 Stuff
(defmodule A)
(assert (x) (y) (z))
(defmodule B)
(assert (a) (b) (c))
(facts)
(facts A)
(facts B)
(facts *)
(clear) ;; New 6.24 stuff
(ppfact)
(ppfact a)
(ppfact 2)
(assert (foo a b c d))
(ppfact 1)
(ppfact 1 nil)
(ppfact 1 t)
(ppfact 1 t FALSE)
(ppfact 1 t TRUE)

(deftemplate bar
   (slot w)
   (slot x (default 3))
   (multislot y)
   (multislot z (default a b)))
(assert (bar))
(ppfact 2 t FALSE)
(ppfact 2 t TRUE)
(assert (bar (w 1) (x 2) (y 3) (z 4)))
(ppfact 3 t FALSE)
(ppfact 3 t TRUE)
(clear)
