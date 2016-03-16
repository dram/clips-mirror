(defglobal ?*x* = (mv-append 1 2 3))
(deffunction foo (?a ?b)
  (printout t "Foo arguments: " ?a " " ?b crlf))
(batch "seqop2.bat")
(set-sequence-operator-recognition TRUE)
(batch "seqop2.bat")
(load seqop.clp)
(watch all)
(unwatch statistics)
(unwatch focus)
(reset)
(run)
(dfnx-seqop-test abc def ghi)
(send [s1] seqop-test abc def ghi)
(+ "abc" "def" "ghi")
(unwatch all)
