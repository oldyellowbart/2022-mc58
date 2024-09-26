import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
from transformers import GPT2Tokenizer, GPT2Model

# Define the GPT-like model using GPT-2 as a base model
class SimpleGPT(nn.Module):
    def __init__(self):
        super(SimpleGPT, self).__init__()
        # Load pre-trained GPT-2 model and tokenizer
        self.gpt_model = GPT2Model.from_pretrained('gpt2')
        self.tokenizer = GPT2Tokenizer.from_pretrained('gpt2')
        self.linear = nn.Linear(self.gpt_model.config.n_embd, self.gpt_model.config.vocab_size)

    def forward(self, input_ids, attention_mask=None):
        # Get hidden states from GPT model
        outputs = self.gpt_model(input_ids, attention_mask=attention_mask)
        hidden_states = outputs.last_hidden_state
        # Pass hidden states through a linear layer to generate logits for each token
        logits = self.linear(hidden_states)
        return logits

    def generate(self, prompt, max_length=50):
        # Tokenize input prompt
        input_ids = self.tokenizer(prompt, return_tensors='pt').input_ids
        generated = input_ids
        
        # Generate new tokens iteratively
        for _ in range(max_length):
            logits = self.forward(generated)
            next_token = torch.argmax(logits[:, -1, :], dim=-1)
            generated = torch.cat((generated, next_token.unsqueeze(0)), dim=1)
            if next_token == self.tokenizer.eos_token_id:
                break
        
        return self.tokenizer.decode(generated[0], skip_special_tokens=True)

# Instantiate the model
model = SimpleGPT()

# Example: generate text from a prompt
prompt = "Once upon a time,"
generated_text = model.generate(prompt, max_length=50)
print("Generated Text: ", generated_text)

# Define optimizer and loss function for training
optimizer = optim.Adam(model.parameters(), lr=0.001)
criterion = nn.CrossEntropyLoss()

# Example: Training loop (for illustration purposes, real training requires large datasets)
def train(model, dataloader, epochs=3):
    model.train()
    for epoch in range(epochs):
        for input_ids, attention_mask in dataloader:
            # Forward pass
            optimizer.zero_grad()
            outputs = model(input_ids, attention_mask)
            loss = criterion(outputs.view(-1, model.gpt_model.config.vocab_size), input_ids.view(-1))
            # Backward pass and optimization
            loss.backward()
            optimizer.step()
        print(f"Epoch {epoch+1}/{epochs}, Loss: {loss.item()}")

# You can add your own dataset and dataloader logic to train the model
