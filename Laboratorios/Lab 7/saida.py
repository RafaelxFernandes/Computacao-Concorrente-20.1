import verificaPC
pc = verificaPC.PC()
# count = 1
pc.inserindo(1)
# Buffer atualizado = 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.insereAcabou(1)
pc.retirando(5)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(5)
pc.retiraBloqueado(4)
pc.retiraBloqueado(3)
pc.retiraBloqueado(2)
pc.retiraBloqueado(1)
# count = 1
pc.inserindo(5)
# Buffer atualizado = 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.insereAcabou(5)
# count = 2
pc.inserindo(4)
# Buffer atualizado = 0 5 4 0 0 0 0 0 0 0 0 0 0 0 0 
pc.insereAcabou(4)
# count = 3
pc.inserindo(3)
# Buffer atualizado = 0 5 4 3 0 0 0 0 0 0 0 0 0 0 0 
pc.insereAcabou(3)
# count = 4
pc.inserindo(2)
# Buffer atualizado = 0 5 4 3 2 0 0 0 0 0 0 0 0 0 0 
pc.insereAcabou(2)
pc.retirando(1)
# Buffer atualizado = 0 0 4 3 2 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(1)
pc.retirando(2)
# Buffer atualizado = 0 0 0 3 2 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(2)
pc.retirando(3)
# Buffer atualizado = 0 0 0 0 2 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(3)
pc.retirando(4)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(4)
# count = 1
pc.inserindo(1)
# Buffer atualizado = 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 
pc.insereAcabou(1)
pc.retirando(1)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(1)
# count = 1
pc.inserindo(1)
# Buffer atualizado = 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 
pc.insereAcabou(1)
# count = 2
pc.inserindo(2)
# Buffer atualizado = 0 0 0 0 0 0 1 2 0 0 0 0 0 0 0 
pc.insereAcabou(2)
pc.retirando(1)
# Buffer atualizado = 0 0 0 0 0 0 0 2 0 0 0 0 0 0 0 
pc.retiraAcabou(1)
pc.retirando(2)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(2)
# count = 1
pc.inserindo(1)
# Buffer atualizado = 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 
pc.insereAcabou(1)
# count = 2
pc.inserindo(3)
# Buffer atualizado = 0 0 0 0 0 0 0 0 1 3 0 0 0 0 0 
pc.insereAcabou(3)
pc.retirando(3)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 3 0 0 0 0 0 
pc.retiraAcabou(3)
pc.retirando(1)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(1)
# count = 1
pc.inserindo(4)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 4 0 0 0 0 
pc.insereAcabou(4)
# count = 2
pc.inserindo(1)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 4 1 0 0 0 
pc.insereAcabou(1)
# count = 3
pc.inserindo(2)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 4 1 2 0 0 
pc.insereAcabou(2)
pc.retirando(4)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 1 2 0 0 
pc.retiraAcabou(4)
pc.retirando(2)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 2 0 0 
pc.retiraAcabou(2)
pc.retirando(1)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(1)
pc.retiraBloqueado(5)
# count = 1
pc.inserindo(5)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 
pc.insereAcabou(5)
pc.retirando(5)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(5)
# count = 1
pc.inserindo(1)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 
pc.insereAcabou(1)
pc.retirando(1)
# Buffer atualizado = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
pc.retiraAcabou(1)
