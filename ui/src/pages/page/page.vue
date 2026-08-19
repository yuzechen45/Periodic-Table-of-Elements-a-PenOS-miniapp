<template>
  <div class="dt-root">
    <div class="dt-header">
      <div class="dt-back" @click="goBack"><text class="dt-back-text">返回</text></div>
      <text class="dt-title">元素详情</text>
    </div>
    <scroller class="dt-scroller" scroll-direction="vertical" :show-scrollbar="true">
      <div v-if="el" class="dt-body">
        <div class="dt-hero" :style="{'background-color': cat.color}">
          <text class="dt-hero-num">{{ el.num }}</text>
          <text class="dt-hero-sym">{{ el.sym }}</text>
          <text class="dt-hero-name">{{ el.name }}</text>
          <text class="dt-hero-py">{{ el.py }} · {{ cat.name }}</text>
        </div>
        <div class="dt-rows">
          <div v-for="(item, i) in infoRows" :key="'i' + i" class="dt-row">
            <text class="dt-label">{{ item.label }}</text>
            <text class="dt-value">{{ item.value }}</text>
          </div>
        </div>
      </div>
    </scroller>
  </div>
</template>

<script>
import { ELEMENTS, CATEGORIES } from '@/data/elements.js'

export default {
  name: 'page',
  data() {
    return {
      num: 1,
    }
  },
  computed: {
    el() {
      return ELEMENTS[this.num]
    },
    cat() {
      return CATEGORIES[this.el.cat]
    },
    infoRows() {
      var el = this.el
      return [
        { label: '原子序数', value: String(el.num) },
        { label: '元素符号', value: el.sym },
        { label: '中文名称', value: el.name },
        { label: '拼音', value: el.py },
        { label: '相对原子质量', value: String(el.mass) },
        { label: '元素分类', value: this.cat.name },
        { label: '电负性', value: el.neg === null ? '—' : String(el.neg) },
        { label: '所属周期', value: '第' + el.period + '周期' },
        { label: '所属族', value: el.group },
        { label: '价电子数', value: String(el.val) },
        { label: '价电子排布', value: el.vconf },
        { label: '常温状态', value: el.state },
        { label: '电子排布', value: el.conf },
        { label: '元素来源', value: el.origin === 'synthetic' ? '人工合成' : '天然存在' },
      ]
    },
  },
  methods: {
    onShow() {
      this.refresh()
    },
    refresh() {
      var options = (this.$page && this.$page.options) || {}
      var num = parseInt(options.num, 10)
      if (!num || num < 1 || num > 118) {
        num = 1
      }
      this.num = num
    },
    goBack() {
      this.$page.finish()
    },
  },
}
</script>

<style scoped>
.dt-root {
  width: 100%;
  height: 100%;
  background-color: #0e1320;
  display: flex;
  flex-direction: column;
}

.dt-header {
  height: 40px;
  display: flex;
  flex-direction: row;
  align-items: center;
  background-color: #161e30;
  padding-left: 8px;
}

.dt-back {
  height: 26px;
  padding-left: 12px;
  padding-right: 12px;
  background-color: #2a3854;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.dt-back-text {
  color: #b9c8e8;
  font-size: 13px;
}

.dt-title {
  flex: 1;
  color: #e8eefc;
  font-size: 16px;
  font-weight: bold;
  text-align: center;
  margin-right: 40px;
}

.dt-scroller {
  flex: 1;
}

.dt-hero {
  margin: 10px;
  border-radius: 12px;
  padding: 12px;
  display: flex;
  flex-direction: column;
  align-items: center;
}

.dt-hero-num {
  font-size: 15px;
  color: #4a5568;
}

.dt-hero-sym {
  font-size: 52px;
  font-weight: bold;
  color: #141a26;
  line-height: 58px;
}

.dt-hero-name {
  font-size: 24px;
  color: #1f2937;
  line-height: 30px;
}

.dt-hero-py {
  font-size: 12px;
  color: #4a5568;
  margin-top: 2px;
}

.dt-rows {
  padding-left: 10px;
  padding-right: 10px;
  padding-bottom: 16px;
}

.dt-row {
  display: flex;
  flex-direction: row;
  align-items: center;
  height: 30px;
  background-color: #151d2e;
  border-radius: 8px;
  margin-bottom: 6px;
  padding-left: 10px;
  padding-right: 10px;
}

.dt-label {
  width: 110px;
  color: #93a3c4;
  font-size: 13px;
}

.dt-value {
  flex: 1;
  color: #eef2fc;
  font-size: 14px;
}
</style>
