import imagematrix

class ResizeableImage(imagematrix.ImageMatrix):
	def best_seam(self):
		
		energy = {}
		for j in range(self.height):
			for i in range(self.width):
				energy[i, j] = self.energy(i, j)

		dp = {}
		back_pointer = {}

		for j in range(self.height):
			for i in range(self.width):
				if j == 0:
					dp[i, j] = energy[i, 0]
					back_pointer[i, j] = None
					continue
				if i == 0:	# left edge
					up_row = [(i, j-1), (i+1, j-1)]
				elif i == self.width-1:		# right edge
					up_row = [(i-1, j-1), (i, j-1)]
				else:	# not edge
					up_row = [(i-1, j-1), (i, j-1), (i+1, j-1)]

				up_energy = [dp[s, t] for s, t in up_row]
				min_energy = min(up_energy)

				dp[i, j] = min_energy + energy[i, j] # update energy

				min_index = [m for m, x in enumerate(up_energy) if x == min_energy]
				min_index = min_index[0]
				up_pix = up_row[min_index]

				back_pointer[i, j] = up_pix		# update back pointer

		# get the seam with lowest energy
		best_end = (0, self.height-1)	# initialize with bottom left corner
		for i in range(self.width):
			if dp[i, self.height-1] < dp[best_end]:
				best_end = (i, self.height-1)

		# get the best_seam path from bottom up
		seam = []
		while best_end is not None:
			seam.append(best_end)
			best_end = back_pointer[best_end]

		seam.reverse()
		return seam

	def remove_best_seam(self):
		self.remove_seam(self.best_seam())
