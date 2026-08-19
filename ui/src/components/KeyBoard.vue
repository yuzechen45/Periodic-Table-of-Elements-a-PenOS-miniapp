<template>
  <div class="kb-root">
    <div v-if="mode === 'abc'">
      <div v-for="(row, ri) in letterRows" :key="'r' + ri" class="kb-row">
        <div v-for="(key, ki) in row" :key="'k' + ri + '-' + ki" class="kb-key" @click="press(key)">
          <text class="kb-key-text">{{ key }}</text>
        </div>
      </div>
      <div class="kb-row">
        <div class="kb-key kb-key-fn" @click="mode = '123'"><text class="kb-key-text">123</text></div>
        <div class="kb-key kb-key-fn" @click="emitClear"><text class="kb-key-text">清空</text></div>
        <div class="kb-key kb-key-fn" @click="emitSearch"><text class="kb-key-text">搜索</text></div>
        <div class="kb-key kb-key-fn" @click="emitClose"><text class="kb-key-text">关闭</text></div>
      </div>
    </div>
    <div v-else>
      <div class="kb-row">
        <div v-for="(key, ki) in digitRow" :key="'d' + ki" class="kb-key" @click="press(key)">
          <text class="kb-key-text">{{ key }}</text>
        </div>
      </div>
      <div class="kb-row">
        <div class="kb-key kb-key-fn" @click="mode = 'abc'"><text class="kb-key-text">ABC</text></div>
        <div class="kb-key kb-key-fn" @click="emitClear"><text class="kb-key-text">清空</text></div>
        <div class="kb-key kb-key-fn" @click="emitBack"><text class="kb-key-text">退格</text></div>
      </div>
      <div class="kb-row">
        <div class="kb-key kb-key-fn" @click="emitSearch"><text class="kb-key-text">搜索</text></div>
        <div class="kb-key kb-key-fn" @click="emitClose"><text class="kb-key-text">关闭</text></div>
        <div class="kb-key kb-key-empty"></div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'KeyBoard',
  data() {
    return {
      mode: 'abc',
      letterRows: [
        ['Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'],
        ['A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'],
        ['Z', 'X', 'C', 'V', 'B', 'N', 'M', '退格'],
      ],
      digitRow: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '0'],
    }
  },
  methods: {
    press(key) {
      if (key === '退格') {
        this.$emit('backspace')
      } else {
        this.$emit('input', key)
      }
    },
    emitClear() {
      this.$emit('clear')
    },
    emitSearch() {
      this.$emit('search')
    },
    emitClose() {
      this.$emit('close')
    },
    emitBack() {
      this.$emit('backspace')
    },
  },
}
</script>

<style scoped>
.kb-root {
  background-color: #131a2b;
  padding: 4px;
}

.kb-row {
  display: flex;
  flex-direction: row;
  margin-bottom: 3px;
}

.kb-key {
  flex: 1;
  height: 26px;
  margin-right: 3px;
  background-color: #26304a;
  border-radius: 6px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.kb-key-text {
  color: #e2e8f6;
  font-size: 15px;
}

.kb-key-fn {
  background-color: #35406b;
}

.kb-key-empty {
  flex: 1;
}

.kb-key:active {
  background-color: #4a5a94;
}
</style>
