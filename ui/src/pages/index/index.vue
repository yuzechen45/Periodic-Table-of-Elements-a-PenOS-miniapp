<template>
  <div class="pg-root">
    <div class="pg-header">
      <text class="pg-title">元素周期表</text>
      <div class="pg-zbtn" @click="zoomBy(-0.1)"><text class="pg-zbtn-text">－</text></div>
      <div class="pg-zval" @click="zoomBy(0)"><text class="pg-zval-text">{{ zoomPct }}%</text></div>
      <div class="pg-zbtn" @click="zoomBy(0.1)"><text class="pg-zbtn-text">＋</text></div>
      <div class="pg-header-btn" @click="legendShow = true"><text class="pg-header-btn-text">图例</text></div>
      <div class="pg-header-btn" @click="openSearch"><text class="pg-header-btn-text">搜索</text></div>
      <div class="pg-header-btn" @click="openSettings"><text class="pg-header-btn-text">设置</text></div>
    </div>

    <scroller class="pg-table" scroll-direction="vertical" :show-scrollbar="true">
      <div class="pt-grid">
        <div class="pt-grow" :style="groupRowStyle()">
          <div v-if="showPeriod" class="pt-period-col" :style="groupRowStyle()"></div>
          <div v-for="(g, gi) in GROUP_HEADERS" :key="'g' + gi" class="pt-slot">
            <text class="pt-group-label" :style="groupTextStyle()">{{ g }}</text>
          </div>
        </div>
        <div v-for="(row, ri) in gridRows" :key="'r' + ri" class="pt-row" :class="ri >= 7 ? 'pt-row-fb' : ''" :style="cellRowStyle(ri)">
          <text v-if="showPeriod" class="pt-period-label" :style="periodLabelStyle(ri)">{{ periodLabel(ri) }}</text>
          <div v-for="(cell, ci) in row" :key="'c' + ri + '-' + ci" class="pt-slot" :style="cellRowStyle(ri)">
            <text v-if="!showPeriod && ri >= 7 && ci === 0" class="pt-fb-label" :style="fbLabelStyle(ri)">{{ ri === 7 ? '镧系' : '锕系' }}</text>
            <div v-else-if="cell && cell.kind === 'ph'" class="pt-cell pt-cell-ph" @click="selectCell(cell.num)">
              <text class="pt-cell-ph-range" :style="textStyle(9)">{{ cell.range }}</text>
              <text class="pt-cell-ph-name" :style="textStyle(10)">{{ cell.name }}</text>
            </div>
            <div v-else-if="cell" class="pt-cell" :style="cellStyle(cell)" @click="selectCell(cell.num)">
              <text class="pt-cell-num" :style="textStyle(9)">{{ cell.num }}</text>
              <text class="pt-cell-sym" :style="textStyle(15)">{{ cell.sym }}</text>
              <text class="pt-cell-name" :style="textStyle(10)">{{ cell.name }}</text>
              <text v-if="showNeg" class="pt-cell-extra" :style="textStyle(8)">{{ cell.neg }}</text>
              <text v-if="showMass" class="pt-cell-extra" :style="textStyle(8)">{{ cell.mass }}</text>
              <text v-if="showVal" class="pt-cell-extra" :style="textStyle(8)">价{{ cell.val }}</text>
              <text v-if="showVconf" class="pt-cell-vconf" :style="vconfTextStyle()">{{ cell.vconf }}</text>
            </div>
          </div>
        </div>
        <div class="pt-pad"></div>
      </div>
    </scroller>

    <div class="pg-infobar">
      <div class="pg-ib-chip" :style="{'background-color': selColor}"></div>
      <div class="pg-ib-main">
        <text class="pg-ib-title">{{ sel.num }} · {{ sel.sym }} · {{ sel.name }}</text>
        <text class="pg-ib-sub">{{ sel.mass }} · {{ catName }} · 第{{ sel.period }}周期</text>
      </div>
      <div class="pg-ib-btn" @click="openDetail"><text class="pg-ib-btn-text">详情</text></div>
    </div>

    <div v-show="legendShow" class="lg-mask" @click="legendShow = false"></div>
    <div v-show="legendShow" class="lg-panel">
      <text class="lg-title">图例 · 元素分类</text>
      <div class="lg-grid">
        <div v-for="cat in categoryList" :key="cat.key" class="lg-row">
          <div class="lg-chip" :style="{'background-color': cat.color}"></div>
          <text class="lg-name">{{ cat.name }}</text>
        </div>
      </div>
      <div class="lg-close" @click="legendShow = false"><text class="lg-close-text">关闭</text></div>
    </div>

    <div v-show="searchShow" class="sr-overlay">
      <div class="sr-bar">
        <div class="sr-input">
          <text :class="query ? 'sr-input-text' : 'sr-input-ph'">{{ query || '输入名称 / 符号 / 序号搜索' }}</text>
        </div>
        <div class="sr-btn" @click="query = ''"><text class="sr-btn-text">清空</text></div>
        <div class="sr-btn" @click="searchShow = false"><text class="sr-btn-text">关闭</text></div>
      </div>
      <scroller class="sr-results" scroll-direction="vertical" :show-scrollbar="true">
        <text v-if="!query" class="sr-hint">支持：中文名拼音（如 tie → 铁）、元素符号（如 Fe）、原子序数（如 26）</text>
        <text v-else-if="results.length === 0" class="sr-hint">没有找到匹配的元素</text>
        <div v-for="r in results" :key="'rs' + r.num" class="sr-row" @click="jumpFromSearch(r.num)">
          <div class="sr-chip" :style="{'background-color': r.color}"></div>
          <text class="sr-main">{{ r.num }} · {{ r.sym }} · {{ r.name }}</text>
          <text class="sr-sub">{{ r.mass }} · {{ r.catName }}</text>
        </div>
      </scroller>
      <key-board @input="onKeyInput" @backspace="onKeyBack" @clear="query = ''" @search="searchShow = false" @close="searchShow = false" />
    </div>
  </div>
</template>

<script>
import KeyBoard from '@/components/KeyBoard.vue'
import { ELEMENTS, CATEGORIES, CATEGORY_LIST, GRID, GROUP_HEADERS, massShort } from '@/data/elements.js'

export default {
  name: 'index',
  components: { KeyBoard },
  data() {
    return {
      sel: ELEMENTS[26],
      legendShow: false,
      searchShow: false,
      query: '',
      GROUP_HEADERS: GROUP_HEADERS,
      zoom: 1,
      showNeg: false,
      showMass: false,
      showVal: false,
      showPeriod: false,
      showVconf: false,
    }
  },
  computed: {
    catName() {
      return CATEGORIES[this.sel.cat].name
    },
    selColor() {
      return CATEGORIES[this.sel.cat].color
    },
    categoryList() {
      return CATEGORY_LIST
    },
    zoomPct() {
      return Math.round(this.zoom * 100)
    },
    extraCount() {
      return (this.showNeg ? 1 : 0) + (this.showMass ? 1 : 0) + (this.showVal ? 1 : 0)
    },
    gridRows() {
      return GRID.map(function (row) {
        return row.map(function (idx) {
          if (idx === null) return null
          if (idx === 'LAN') return { kind: 'ph', num: 57, range: '57-71', name: '镧系' }
          if (idx === 'ACT') return { kind: 'ph', num: 89, range: '89-103', name: '锕系' }
          var el = ELEMENTS[idx]
          return {
            num: el.num,
            sym: el.sym,
            name: el.name,
            color: CATEGORIES[el.cat].color,
            neg: el.neg === null ? '-' : el.neg.toFixed(2),
            mass: massShort(el.mass),
            val: el.val,
            vconf: el.vconf,
          }
        })
      })
    },
    results() {
      var q = this.query.trim()
      if (!q) return []
      var ql = q.toLowerCase()
      var scored = []
      ELEMENTS.forEach(function (el) {
        if (!el) return
        var score = 0
        if (String(el.num) === q) score = 1
        else if (el.sym.toLowerCase() === ql) score = 2
        else if (el.name === q) score = 3
        else if (el.py === ql) score = 4
        else if (el.sym.toLowerCase().indexOf(ql) === 0) score = 5
        else if (el.py.indexOf(ql) === 0) score = 6
        else if (String(el.num).indexOf(q) === 0) score = 7
        else if (el.name.indexOf(q) === 0) score = 8
        if (score > 0) scored.push({ el: el, score: score })
      })
      scored.sort(function (a, b) {
        if (a.score !== b.score) return a.score - b.score
        return a.el.num - b.el.num
      })
      return scored.slice(0, 30).map(function (item) {
        var el = item.el
        return {
          num: el.num,
          sym: el.sym,
          name: el.name,
          mass: el.mass,
          color: CATEGORIES[el.cat].color,
          catName: CATEGORIES[el.cat].name,
        }
      })
    },
  },
  methods: {
    textStyle(base) {
      var s = Math.round(base * this.zoom * 10) / 10
      return { fontSize: s + 'px', lineHeight: s + 'px' }
    },
    vconfTextStyle() {
      var s = Math.round(7 * this.zoom * 10) / 10
      var lh = Math.round(8 * this.zoom * 10) / 10
      return { fontSize: s + 'px', lineHeight: lh + 'px' }
    },
    vconfLines(ri) {
      if (!this.showVconf) return 0
      var cellW = (640 - 8 - 34 - (this.showPeriod ? 16 : 0)) / 18
      var max = 0
      var row = GRID[ri]
      for (var i = 0; i < row.length; i++) {
        var idx = row[i]
        if (idx === null || idx === 'LAN' || idx === 'ACT') continue
        var len = ELEMENTS[idx].vconf.length
        var lines = Math.ceil(len * 0.8 * 7 * this.zoom / cellW)
        if (lines > max) max = lines
      }
      return max
    },
    rowHeight(ri) {
      var h = (36 + 9 * this.extraCount + 8 * this.vconfLines(ri)) * this.zoom
      return Math.round(h * 10) / 10
    },
    cellRowStyle(ri) {
      return { height: this.rowHeight(ri) + 'px' }
    },
    groupRowStyle() {
      var h = Math.round(14 * this.zoom * 10) / 10
      return { height: h + 'px' }
    },
    groupTextStyle() {
      var s = Math.round(8 * this.zoom * 10) / 10
      var h = Math.round(14 * this.zoom * 10) / 10
      return { fontSize: s + 'px', lineHeight: h + 'px' }
    },
    fbLabelStyle(ri) {
      var s = Math.round(9 * this.zoom * 10) / 10
      var h = this.rowHeight(ri)
      return { fontSize: s + 'px', lineHeight: h + 'px' }
    },
    periodLabelStyle(ri) {
      var s = Math.round(9 * this.zoom * 10) / 10
      var h = this.rowHeight(ri)
      return { fontSize: s + 'px', lineHeight: h + 'px' }
    },
    periodLabel(ri) {
      if (ri === 7) return '镧系'
      if (ri === 8) return '锕系'
      return String(ri + 1)
    },
    cellStyle(cell) {
      var style = { backgroundColor: cell.color, borderWidth: 0, borderColor: '#ffffff' }
      if (cell.num === this.sel.num) {
        style.borderWidth = 2
      }
      return style
    },
    zoomBy(d) {
      var z = this.zoom
      if (d === 0) z = 1
      else z = z + d
      if (z < 0.6) z = 0.6
      if (z > 1.5) z = 1.5
      this.zoom = Math.round(z * 10) / 10
      this.saveZoom()
    },
    saveZoom() {
      var p = $falcon.jsapi.storage.setStorage({ key: 'pt-zoom', data: String(this.zoom) })
      if (p && p.catch) p.catch(function () {})
    },
    async loadSettings() {
      var self = this
      var pairs = [
        ['pt-neg', function (d) { self.showNeg = d === '1' }],
        ['pt-mass', function (d) { self.showMass = d === '1' }],
        ['pt-val', function (d) { self.showVal = d === '1' }],
        ['pt-period', function (d) { self.showPeriod = d === '1' }],
        ['pt-vconf', function (d) { self.showVconf = d === '1' }],
        ['pt-zoom', function (d) {
          var z = parseFloat(d)
          if (z >= 0.6 && z <= 1.5) self.zoom = z
        }],
      ]
      for (var i = 0; i < pairs.length; i++) {
        try {
          var res = await $falcon.jsapi.storage.getStorage({ key: pairs[i][0] })
          pairs[i][1](res && res.data)
        } catch (err) {}
      }
    },
    onShow() {
      this.loadSettings()
    },
    selectCell(num) {
      this.sel = ELEMENTS[num]
    },
    openDetail() {
      $falcon.navTo('page', { num: this.sel.num })
    },
    openSettings() {
      $falcon.navTo('settings', {})
    },
    openSearch() {
      this.query = ''
      this.searchShow = true
    },
    onKeyInput(key) {
      if (this.query.length < 12) {
        this.query += key
      }
    },
    onKeyBack() {
      this.query = this.query.slice(0, -1)
    },
    jumpFromSearch(num) {
      this.sel = ELEMENTS[num]
      this.searchShow = false
      $falcon.navTo('page', { num: num })
    },
  },
}
</script>

<style scoped>
.pg-root {
  width: 100%;
  height: 100%;
  background-color: #0e1320;
  display: flex;
  flex-direction: column;
}

.pg-header {
  height: 40px;
  display: flex;
  flex-direction: row;
  align-items: center;
  background-color: #161e30;
  padding-left: 10px;
  padding-right: 8px;
}

.pg-title {
  flex: 1;
  color: #e8eefc;
  font-size: 17px;
  font-weight: bold;
}

.pg-zbtn {
  width: 24px;
  height: 26px;
  margin-left: 4px;
  background-color: #2a3854;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.pg-zbtn-text {
  color: #b9c8e8;
  font-size: 13px;
}

.pg-zval {
  width: 46px;
  height: 26px;
  margin-left: 4px;
  background-color: #3557a8;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.pg-zval-text {
  color: #eef2ff;
  font-size: 12px;
}

.pg-header-btn {
  height: 26px;
  padding-left: 10px;
  padding-right: 10px;
  margin-left: 6px;
  background-color: #2a3854;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.pg-header-btn-text {
  color: #b9c8e8;
  font-size: 13px;
}

.pg-table {
  flex: 1;
}

.pt-grid {
  padding: 4px;
}

.pt-grow {
  display: flex;
  flex-direction: row;
  height: 14px;
  margin-bottom: 2px;
}

.pt-row {
  display: flex;
  flex-direction: row;
  height: 36px;
  margin-bottom: 2px;
}

.pt-row-fb {
  margin-top: 4px;
}

.pt-slot {
  flex: 1;
  height: 36px;
  margin-right: 2px;
}

.pt-group-label {
  width: 100%;
  text-align: center;
  font-size: 8px;
  line-height: 14px;
  color: #6b7a9e;
}

.pt-fb-label {
  width: 100%;
  text-align: right;
  font-size: 9px;
  line-height: 36px;
  color: #8c9cc0;
}

.pt-period-col {
  width: 16px;
}

.pt-period-label {
  width: 16px;
  text-align: center;
  font-size: 9px;
  line-height: 36px;
  color: #8c9cc0;
}

.pt-cell {
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  border-radius: 4px;
}

.pt-cell:active {
  opacity: 0.75;
}

.pt-cell-num {
  font-size: 9px;
  line-height: 9px;
  color: #37404f;
}

.pt-cell-sym {
  font-size: 15px;
  line-height: 16px;
  font-weight: bold;
  color: #141a26;
}

.pt-cell-name {
  font-size: 10px;
  line-height: 10px;
  color: #2a3242;
}

.pt-cell-extra {
  font-size: 8px;
  line-height: 8px;
  color: #37404f;
}

.pt-cell-vconf {
  width: 100%;
  text-align: center;
  font-size: 7px;
  line-height: 8px;
  color: #37404f;
}

.pt-cell-ph {
  background-color: #232c42;
}

.pt-cell-ph-range {
  font-size: 9px;
  line-height: 10px;
  color: #7d8aa8;
}

.pt-cell-ph-name {
  font-size: 10px;
  line-height: 12px;
  color: #c3cee6;
}

.pt-pad {
  height: 6px;
}

.pg-infobar {
  height: 42px;
  background-color: #151d2e;
  display: flex;
  flex-direction: row;
  align-items: center;
  padding-left: 8px;
  padding-right: 8px;
}

.pg-ib-chip {
  width: 16px;
  height: 16px;
  border-radius: 4px;
  margin-right: 8px;
}

.pg-ib-main {
  flex: 1;
  display: flex;
  flex-direction: column;
}

.pg-ib-title {
  color: #f0f4fc;
  font-size: 14px;
  line-height: 16px;
}

.pg-ib-sub {
  color: #93a3c4;
  font-size: 11px;
  line-height: 13px;
}

.pg-ib-btn {
  width: 52px;
  height: 26px;
  background-color: #3557a8;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.pg-ib-btn-text {
  color: #eef2ff;
  font-size: 13px;
}

.lg-mask {
  position: fixed;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.6);
  z-index: 90;
}

.lg-panel {
  position: fixed;
  left: 15%;
  top: 10%;
  width: 70%;
  background-color: #182136;
  border-radius: 12px;
  padding: 12px;
  z-index: 91;
  display: flex;
  flex-direction: column;
}

.lg-title {
  color: #e8eefc;
  font-size: 15px;
  font-weight: bold;
  text-align: center;
  margin-bottom: 8px;
}

.lg-grid {
  display: flex;
  flex-direction: row;
  flex-wrap: wrap;
}

.lg-row {
  width: 50%;
  height: 22px;
  display: flex;
  flex-direction: row;
  align-items: center;
}

.lg-chip {
  width: 14px;
  height: 14px;
  border-radius: 3px;
  margin-right: 8px;
}

.lg-name {
  color: #c6d2ec;
  font-size: 13px;
}

.lg-close {
  height: 28px;
  margin-top: 10px;
  background-color: #2a3854;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.lg-close-text {
  color: #b9c8e8;
  font-size: 13px;
}

.sr-overlay {
  position: fixed;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  background-color: #0e1320;
  z-index: 100;
  display: flex;
  flex-direction: column;
}

.sr-bar {
  height: 40px;
  display: flex;
  flex-direction: row;
  align-items: center;
  padding-left: 8px;
  padding-right: 8px;
  background-color: #161e30;
}

.sr-input {
  flex: 1;
  height: 28px;
  background-color: #0b101c;
  border-radius: 8px;
  display: flex;
  align-items: center;
  padding-left: 8px;
}

.sr-input-text {
  color: #dde6f8;
  font-size: 14px;
}

.sr-input-ph {
  color: #5c6a8c;
  font-size: 13px;
}

.sr-btn {
  height: 28px;
  padding-left: 10px;
  padding-right: 10px;
  margin-left: 6px;
  background-color: #2a3854;
  border-radius: 8px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.sr-btn-text {
  color: #b9c8e8;
  font-size: 13px;
}

.sr-results {
  flex: 1;
  padding: 4px;
}

.sr-hint {
  color: #7d8aa8;
  font-size: 13px;
  margin: 10px;
  line-height: 20px;
}

.sr-row {
  display: flex;
  flex-direction: row;
  align-items: center;
  height: 34px;
  background-color: #151d2e;
  border-radius: 8px;
  margin-bottom: 4px;
  padding-left: 8px;
  padding-right: 8px;
}

.sr-row:active {
  background-color: #22304f;
}

.sr-chip {
  width: 14px;
  height: 14px;
  border-radius: 3px;
  margin-right: 8px;
}

.sr-main {
  color: #eef2fc;
  font-size: 14px;
}

.sr-sub {
  flex: 1;
  color: #93a3c4;
  font-size: 11px;
  text-align: right;
}
</style>
