class PC:

    def __init__(self):
        self.count = 0
        self.N = 15

    def insereBloqueado(self, id):
        if(self.count != N):
            print("--- ERRO: produtor " + str(id) + " bloqueado mas buffer não está cheio!")

    def retiraBloqueado(self, id):
        if(self.count != 0):
            print("--- ERRO: consumidor " + str(id) +  " bloqueado mas buffer não está vazio!")
