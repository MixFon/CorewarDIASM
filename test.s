.name "Kittystrophic"
.comment "Jan-ken-pon (じゃんけんぽん)
Ce qui reste éternellement incompréhensible dans la nature, c’est qu’on puisse la comprendre.
Celui qui ne peut plus éprouver ni étonnement ni surprise, est pour ainsi dire mort : ses yeux sont éteints. – Albert Einstein
Si vous voulez trouver les secrets de l'univers pensez en termes de fréquences, d'énergies et de vibrations. - Nikola Tesla
Pas la pour etre ici. - Silicon Val[list]
Dieu est la plus belle invention.
Tapis dans l'ombre je veille. - Minou"

	live %-102
	live %1
	live %2147483647
	live %2147483647
	live %-2147483648
	live %252645135
	live %252645135
	fork %34
	fork %-34
	fork %327678
	fork %-32767
	zjmp %34
	zjmp %-34
	zjmp %32767
	zjmp %-32767
	lfork %34
	lfork %-34
	lfork %32767
	lfork %-32767
	add r1, r2, r3
	add r16, r12, r1
	sub r1, r2, r3
	sub r16, r12, r1
	aff r16
	aff r1
	and r1, r2, r3
	and %2147483647, 2147483647, r4
	and %-2147483648, -214748364, r5
	or %2147483647, 2147483647, r4
	or %-2147483648, -214748364, r5
	xor %2147483647, 2147483647, r4
	xor %-2147483648, -214748364, r5
	ld %234, r4
	ld %-2147483648, r5
	ld 32767,  r4
	ld -32767, r5
	lld %1, r4
	lld %2, r5
	lld 32767,  r4
	lld -32767, r5
	st r1, r3
	st r1, r3
	st r1, 32
	st r1, -3
	sti r1, r3 , r5
	sti r1, r3 , r5
	sti r1, r3 , r5
	sti r1, %1, %2
	sti r1, r3, r4
	sti r1, 4, %3
	ldi r1, r3, r3
	ldi %13, %23, r3
	ldi 24, %12, r4
	lldi r1, r3, r3
	lldi %13, %23, r3
	lldi 24, %12, r4
	

