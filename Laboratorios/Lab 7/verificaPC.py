class PC:

    def __init__(self):
        self.produtoras = 0
        self.consumidoras = 0

    def insereBloqueado(self, id):
        if(self.consumidoras == 0):
            print("--- ERRO: produtor " + str(id) + " bloqueado quando não há consumidores!")

    def inserindo(self, id):
        if(self.consumidoras > 0):
            print("--- ERRO: produtor " + str(id) + " está inserindo quando há consumidor retirando!")
        self.produtoras += 1

    def insereAcabou(self, id):
        self.produtoras -= 1

    def retiraBloqueado(self, id):
        if((self.produtoras == 0) and (self.consumidoras == 0)):
            print("--- ERRO: consumidor " + str(id) +  " bloqueado quando não há produtores nem consumidores!")

    def retirando(self, id):
        if((self.produtoras > 0) or (self.consumidoras > 0)):
            print("--- ERRO: consumidor " + str(id) + " está retirando quando há produtores ou consimidores atuando!")
        self.consumidoras += 1
    
    def retiraAcabou(self, id):
        self.consumidoras -= 1